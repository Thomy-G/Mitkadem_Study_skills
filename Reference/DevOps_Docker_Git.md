# DevOps, Docker, & Git Internals — Study Reference Guide

---

## 1. Low-Level Git Object Mechanics

Git is a content-addressable database. Under the hood, everything is stored inside `.git/objects/` using a zlib-compressed representation named after its SHA-1 hash (a 40-character hexadecimal string). The first 2 characters of the hash form the subdirectory name (e.g. `.git/objects/4b/`), and the remaining 38 characters form the filename.

### 1.1 The 3 Primary Git Object Types
Every object starts with a header defining its type and content size, terminated by a null byte (`\0`), followed by the raw contents.

#### 1. Blob (Binary Large Object)
Stores file data only. It contains no metadata, creation dates, or filenames.
*   **Header Format**: `blob <size_in_bytes>\0<file_contents>`
*   If two identical files exist in different directories, Git stores only **one** blob object since their hashes are identical.

#### 2. Tree
Represents a directory directory. It maps file and folder names to their permissions, file types, and object hashes (blobs or other trees).
*   **Header Format**: `tree <size_in_bytes>\0`
*   **Entries**: `<file_mode> <filename>\0<binary_sha1_hash>`

#### 3. Commit
Stores metadata, parent pointers, and the root directory tree pointer.
*   **Header Format**: `commit <size_in_bytes>\0`
*   **Body Content**:
    ```
    tree 4b825dc642cb6eb9a0accbad204d0b27e5554f25
    parent 8b1f5fc642cb6eb9a0accbad204d0b27e5554f21 (Omitted for initial commit)
    author User Name <email@example.com> 1782992048 +0300
    committer User Name <email@example.com> 1782992048 +0300
    
    Commit message here
    ```

---

### 1.2 Git Command Walkthrough: Tracking Object Creation

Assume a developer performs the following commands in an empty repository:
```bash
$ git init
$ echo "Hello" > a.txt
$ git add a.txt
$ git commit -m "init"
```

1.  **`git add a.txt`**:
    *   Compresses the string `"Hello\n"` to zlib.
    *   Calculates SHA-1 hash for `blob 6\0Hello\n` -> e.g., `ebe352b36b1309ec2c35a92a54b38cf48ff2f741`.
    *   Creates `.git/objects/eb/e352b36b1309ec2c35a92a54b38cf48ff2f741`.
2.  **`git commit -m "init"`**:
    *   Creates a **Tree Object** representing the root directory index, linking `"a.txt"` to the blob hash `ebe352b...`.
    *   Creates a **Commit Object** pointing to this root tree and listing metadata.
    *   *Total objects created*: 3 (1 Blob, 1 Tree, 1 Commit).

#### Modifying `a.txt` and committing again:
```bash
$ echo "Hello World" > a.txt
$ git add a.txt
$ git commit -m "update"
```
*   **Step 1**: Git creates a **new blob** containing `"Hello World\n"`. (The old blob remains untouched).
*   **Step 2**: Git creates a **new tree** mapping `a.txt` to the new blob hash.
*   **Step 3**: Git creates a **new commit** pointing to the new tree and setting the `parent` header to the first commit hash.
*   *Total objects created in session*: 3 new objects (Cumulative repository total: 6 objects).

---

## 2. Docker Cache Optimizations & Multi-Stage Builds

### 2.1 The Dockerfile Layering and Caching Engine
Every command inside a `Dockerfile` (e.g. `COPY`, `RUN`, `ENV`) generates a read-only container layer. Docker caches these layers locally.
*   **Cache Invalidation**: During a build, if a layer's dependencies change (e.g., changes to source files matched by `COPY`), that layer's cache is invalidated.
*   **Downstream Consequence**: **Once a layer cache is invalidated, all subsequent layers must rebuild from scratch**.

#### Unoptimized Configuration:
```dockerfile
FROM node:alpine
COPY . /app
WORKDIR /app
RUN npm install
CMD ["node", "server.js"]
```
*   *Why this is bad*: The code files (`COPY . /app`) are copied *before* `RUN npm install` runs. If you edit a single code file, the cache for the `COPY` layer invalidates. Consequently, `npm install` must rebuild from scratch, downloading all packages again.

#### Optimized Configuration:
```dockerfile
FROM node:alpine
WORKDIR /app
# Copy only package manifests first
COPY package*.json ./
# Install dependencies (cached unless package.json changes)
RUN npm install
# Copy remaining source code files
COPY . .
CMD ["node", "server.js"]
```
*   *Why this is good*: Changing source files only invalidates the `COPY . .` layer. The dependencies remain cached and do not rebuild.

---

### 2.2 Multi-Stage Production Builds
Multi-stage builds permit dividing the build process into distinct temporary stages, copying only the necessary artifacts to the final production image.

```dockerfile
# ==========================================
# STAGE 1: Compilation/Build Environment
# ==========================================
FROM node:18-alpine AS builder
WORKDIR /app

# Install dependencies
COPY package*.json ./
RUN npm ci --silent

# Copy source and build static bundle
COPY . .
RUN npm run build

# ==========================================
# STAGE 2: Lightweight Production Runtime
# ==========================================
FROM nginx:1.25-alpine

# Copy only the compiled build files from Stage 1
COPY --from=builder /app/build /usr/share/nginx/html

# Expose port and start Web Server
EXPOSE 80
CMD ["nginx", "-g", "daemon off;"]
```

#### Key Multi-Stage Benefits:
1.  **Small Image Size**: The final production image excludes node build engines, compiler utilities, and devDependencies, drastically reducing storage size.
2.  **Enhanced Security**: Excludes raw source code and development utilities from the production environment, reducing the system's attack surface.
