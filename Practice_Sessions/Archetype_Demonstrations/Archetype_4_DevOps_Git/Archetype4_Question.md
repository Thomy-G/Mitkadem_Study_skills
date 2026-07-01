# Archetype 4: Infrastructure, DevOps, & Git Low-Level Internals (20 Points)

## Section A: Dockerfile Layer Caching Optimization (10 Points)
Consider the following unoptimized `Dockerfile` for a React web application:

```dockerfile
FROM node:18-alpine
COPY . /app
WORKDIR /app
RUN npm install
RUN npm run build
EXPOSE 8080
CMD ["npm", "start"]
```

1. Identify the architectural flaw in this Dockerfile regarding layer caching and build efficiency.
2. Explain the consequences on execution build time when a developer modifies a single CSS/JS source file.
3. Write the fully optimized version of this `Dockerfile` to maximize build speeds.

---

## Section B: Git Low-Level Object Storage (10 Points)
A developer runs the following commands inside an empty directory:

```bash
$ git init
$ echo "Hello Systems Programming" > doc.txt
$ git add doc.txt
$ git commit -m "initial commit"
```

Explain exactly how the Git database (`.git/objects/`) changes after these commands are executed:
1. State the exact number of objects created.
2. Name the type tag (`blob`, `tree`, `commit`) of each object created.
3. Describe the contents and metadata stored within each object.
