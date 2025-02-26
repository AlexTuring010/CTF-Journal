# Copying Files from a Docker Container to the Host

If you're inside a Docker container and want to **copy a file's contents** to your host machine, follow these steps:

### **1. Find the Container ID**

To get the **container ID** of the running Docker containers, use the following command:

```bash
docker ps
```

**Explanation:**

- `docker ps` stands for **"process status"**, and it lists all the currently running Docker containers.
- This will provide you with details like the container ID, image name, and status.

### **2. Copy the File from the Container**

Once you have the **container ID**, you can use `docker cp` to copy a file from the container to your host.

```bash
docker cp <container_id>:<path_in_container> <path_on_host>
```

- `<container_id>`: The ID or name of the running container (found from `docker ps`).
- `<path_in_container>`: The path to the file inside the container.
- `<path_on_host>`: The location where the file should be copied on your host machine.

### **Example:**

1. First, find the running containers with `docker ps`:

```bash
$ docker ps
CONTAINER ID   IMAGE         COMMAND     CREATED       STATUS         PORTS     NAMES
d4e2f1a2b3c4   ubuntu        "/bin/bash"  5 minutes ago Up 5 minutes             my_container
```

2. Copy the file from the container to your current directory on the host:

```bash
docker cp d4e2f1a2b3c4:/home/ubuntu/output.txt .
```

This will copy the `output.txt` file from `/home/ubuntu/` inside the container to the current directory (`.`) on your host machine.

---

### **Notes:**

- Make sure the file path inside the container (`/home/ubuntu/output.txt`) is correct.
- The `docker cp` command works with both running and stopped containers.
