# Solution: Archetype 4 — DevOps, Docker & Git Low-Level Internals

Here is the explanation and solution sheet for the Archetype 4 tasks.

---

## Section A: Dockerfile Layer Caching Optimization

### 1. The Architectural Flaw
Docker builds images sequentially using **Layer Caching**. If a layer's contents have not changed, Docker skips execution and uses the cached layer.
- In the unoptimized Dockerfile, `COPY . /app` is placed **before** `RUN npm install`. 
- If a developer edits a single code file, the cache for the `COPY` layer is invalidated. 
- Once a layer's cache is invalidated, **all subsequent layers must be rebuilt from scratch**. Therefore, Docker is forced to re-run `RUN npm install`, downloading and installing all package dependencies again.

### 2. Consequences
Every simple source code or CSS update invalidates the cache, forcing redundant dependency installs (`npm install`) that waste bandwidth and cause slow build times (often taking minutes instead of seconds).

### 3. Optimized Dockerfile
By separating dependency installation from code copying, we preserve the package installation cache:

```dockerfile
FROM node:18-alpine
WORKDIR /app

# Copy dependency specifications first
COPY package*.json ./

# Install dependencies (this layer is cached unless package.json changes)
RUN npm install

# Copy the rest of the application files
COPY . .

# Run the build
RUN npm run build

EXPOSE 8080
CMD ["npm", "start"]
```

---

## Section B: Git Low-Level Object Storage

When the developer commits the file, Git generates exactly **3 compressed objects** in its content-addressable filesystem database (`.git/objects/`).

```
.git/objects/
├── [Blob Object]   (Created on 'git add')
├── [Tree Object]   (Created on 'git commit')
└── [Commit Object] (Created on 'git commit')
```

### 1. Object 1: Blob (Binary Large Object)
- **Trigger**: Created when running `git add doc.txt`.
- **Contents**: The raw, compressed content of the file: `"Hello Systems Programming\n"`.
- **Metadata**: None. Blobs **do not store** the filename, directory path, or file permissions. The object is named after the SHA-1 hash of its content.

### 2. Object 2: Tree
- **Trigger**: Created when running `git commit`.
- **Contents**: A directory listing. It maps the filename (`doc.txt`) and file permission modes (`100644` for regular files) to the SHA-1 hash of the corresponding blob object.
- **Purpose**: Reconstructs the directory structure and file names for this snapshot.

### 3. Object 3: Commit
- **Trigger**: Created when running `git commit`.
- **Contents**: 
  - A pointer to the root **Tree** object (describing the project directory state).
  - Author and Committer names/emails and timestamps.
  - A pointer to parent commits (in this case, there are no parent commits since it is the initial commit).
  - The commit message: `"initial commit"`.
