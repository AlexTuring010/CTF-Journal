# Docker Introduction

Docker is an open-source platform used for automating the deployment, scaling, and management of applications inside containers. Containers allow developers to package an application with all its dependencies (like libraries and configurations) into a single, portable unit that can run anywhere.

## Key Concepts

- **Containers**: Lightweight, standalone, executable packages of software that include everything needed to run an application (code, runtime, libraries, etc.).
- **Docker Image**: A read-only template used to create Docker containers. An image can be thought of as a snapshot of an application and its environment.
- **Dockerfile**: A text file that contains instructions for building a Docker image. It specifies things like which base image to use, dependencies to install, and how to run the application.
- **Docker Hub**: A cloud-based registry that allows you to share, store, and access Docker images.

## Getting Started

### Installation

You can install Docker on Windows, macOS, or Linux. Follow the instructions on the official Docker website for your platform:

- [Docker Installation Guide](https://docs.docker.com/get-docker/)

### Basic Commands

- `docker --version`: Check the installed version of Docker.
- `docker pull <image_name>`: Download a Docker image from Docker Hub.
- `docker build -t <image_name> .`: Build a Docker image from a Dockerfile in the current directory.
- `docker run <image_name>`: Run a container from an image.
- `docker ps`: List all running containers.
- `docker stop <container_id>`: Stop a running container.

### Example: Running a Web Application

1. Pull a base image:

   ```bash
   docker pull node:14
   ```

2. Create a simple `Dockerfile` for a Node.js app:

   ```dockerfile
   FROM node:14
   WORKDIR /app
   COPY . /app
   RUN npm install
   CMD ["node", "index.js"]
   ```

3. Build the Docker image:

   ```bash
   docker build -t node-app .
   ```

4. Run the container:
   ```bash
   docker run -p 3000:3000 node-app
   ```

The application will now be accessible on `http://localhost:3000`.

## Resources

- [Official Docker Documentation](https://docs.docker.com/)
- [Docker for Beginners - YouTube](https://youtu.be/eGz9DS-aIeY?si=KRdMiikF5WG53Sza)
