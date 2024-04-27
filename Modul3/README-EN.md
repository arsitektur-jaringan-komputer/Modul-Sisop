# Module 3 - Docker

## Learning Outcomes

1. Understand containerization and how it differs from virtualization
2. Understand the basic architecture of docker
3. Can bind file system with docker

## Prerequisite

1. Installing docker, you can follow this [module](https://github.com/arsitektur-jaringan-komputer/Pelatihan-Docker/tree/master/0.%20Prasyarat)

## Table of Contents

- [i. Learning Outcomes](#learning-outcomes)
- [ii. Prerequisite](#prerequisite)
- [iii. Table of Contents](#table-of-contents)
- [1. Introduction](#introduction)
  - [1.1. Why We Need Process Isolation](#why-we-need-process-isolation)
  - [1.2. Difference between Containerization and Virtualization](#difference-between-containerization-and-virtualization)
  - [1.3. Definition of Docker](#definition-of-docker)
- [2. Docker Architecture](#docker-architecture)
  - [2.1. Docker Daemon](#docker-daemon)
  - [2.2. Docker Client](#docker-client)
  - [2.3. Docker Objects](#docker-objects)
  - [2.4. Docker Registry](#docker-registry)
- [3. Docker Service Basics](#docker-service-basics)
  - [3.1. Dockerfile](#dockerfile)
  - [3.2. Docker Image](#docker-image)
  - [3.3. Docker Container](#docker-container)
  - [3.4. Dockerfile Example](#dockerfile-example)
  - [3.5. Docker Hub](#docker-hub)
- [4. Advanced Docker Service](#docker-service-advanced)
  - [4.1. Docker Compose](#docker-compose)
  - [4.2. Docker Data Management](#docker-data-management)
- [iv. Reference](#reference)

## Introduction

### Why We Need Process Isolation

Have you ever compiled a program while listening to MP3 music on your laptop? You might assume your program and MP3 player wouldn't interact. However, in reality, they may be running on exactly the same processor. So, how does the system ensure that your compiled program never overwrites your MP3 player program? Why don't these programs affect each other?

They run in different processes, each with its own address space. This address space is what provides process isolation.

The concept of process isolation is that whatever one process does should not affect or interfere with another, unless the processes agree to communicate (for example, over a pipe, refer to module 2). 

Imagine if one process fails. Without process isolation, the failure of this one process can damage other processes, and can even cause the entire system to fail. Very dangerous, right? That's why process isolation is so important.

### Difference between Containerization and Virtualization

Now we will get to know containerization and virtualization. 

Containerization is a method for packaging an application and its dependencies into a container that can be run consistently across different computing environments, without the need to change the code or configuration of the application itself. Containers are portable, lightweight, and isolated units that package applications, libraries, and configurations into a single entity that can be run in different environments, such as on-premises, cloud, or data center. 

Meanwhile, virtualization is a technology that allows the creation of virtual machines within a single physical server. By using a hypervisor, virtualization allows the management of multiple operating systems or applications that run independently. The basic concept of virtualization involves isolating resources between virtual machines, so that each virtual machine can operate as if it were a separate physical machine.

![virtualization-vs-containerization](assets/virtualization-vs-containerization.jpg)

Both virtualization and containerization use system isolation to run multiple operating systems or applications separately on the same hardware. However, virtualization and containerization have key differences. Virtualization creates a virtual copy of the entire physical computer, which can take up a lot of resources. Containerization simply packages applications and their dependencies in containers within the same host, sharing the same OS kernel, so it is lighter and more efficient than virtualization.

### Definition of Docker

![docker-logo](assets/docker-logo.png)

One technology for creating containers that share a host operating system is Docker. With Docker, developers can create consistent and portable containers, which can be run across different computing environments, including local machines, cloud servers, or different development and production environments. Docker allows applications and their dependencies to be isolated, so that applications can be run consistently across different environments without disrupting the host operating system or other applications.

Imagine we have a Node application. We will need a server that uses the same version of Node as our application. We also need to install the dependencies that the application uses. Thus, this application will be able to run on our computer. However, imagine if someone else with a different computer runs our application with a different version of Node, there could be errors. By using Docker, we can containerize the application and its dependencies to run in various environments without the need for reconfiguration, avoiding this case.

## Docker Architecture

![architecture](assets/architecture.png)

### Docker Daemon

Docker Daemon is a component that runs in the background on the host and is responsible for running and managing Docker Objects such as images, containers, networks, and others. Docker Daemon is a process that runs inside the host operating system and receives commands from Docker Client to create, run, stop, and manage Docker Objects. Docker Daemon is also responsible for managing host resources such as CPU, memory, and network used by Docker Object.

### Docker Client

Docker Client is a command-line or GUI-based user interface used to interact with Docker. Docker Client allows users to execute Docker commands to create, manage, and control services in Docker. Docker Client communicates with the Docker Daemon to send Docker commands and receive the output of running Docker services.

### Docker Objects

Docker Objects are the basic components found in Docker. Some examples of Docker Objects include image, container, volume, and network which will be explained in the next module.

### Docker Registry

Docker Registry is a repository used to store and share Docker images. Docker Registry serves as a storage place for Docker Image that can be accessed by Docker users from various locations. Docker Hub, which is a Docker public registry, is one example of a Docker Registry that is often used to store and share Docker images publicly. Besides Docker Hub, users can also create a private Docker Registry to store Docker Image.

### Docker Service Basics

![docker-service-dasar](assets/docker-service-dasar.png)

### Dockerfile

A Dockerfile is a text file that contains instructions for building a Docker Image. Various components and configurations needed to create an image, such as the base image used, commands that must be run, files that must be copied, and environment variables that need to be set, can be specified with a Dockerfile. 

The advantages of using Dockerfile include allowing users to create images in a consistent and well-documented way, being able to replicate the same settings and configurations every time they build an image even though the environment is different, and allowing the use of the concept of modularity in building images so that image components can be replaced without the need to rebuild the entire image.

To build a Dockerfile into a Docker Image, you can use the following command in the directory containing the Dockerfile.

```
docker build -t <image-name> .
```

#### Dockerfile Commands

Here are some important commands and their explanations that can be implemented in Dockerfile.

| Command | Description |
| ------------ | ------------ |
| `FROM` | Specifies the base image that will be used for the build. |
| `COPY` | Copies files or folders from the host into the image. |
| `ADD` | Copies a file or folder from the host into the image, can also be used to download files from a URL and extract them into the image. |
| `RUN` | Executes commands on the current layer and creates a new image. |
| `CMD` | Specifies the default commands that will be run when the container is started. |
| `ENTRYPOINT` | Specifies the command to be executed when the container is started, can also be overwritten by the command when the container is run. |
| `ENV` | Specifies the environment variable inside the container. |
| `EXPOSE` | Specifies the port to be exposed from the container to the host. |
| `VOLUME` | Specifies the directory that will be mounted as a volume in the container. |

### Docker Image

A docker image is a template for running a docker container. This image contains an operating system and applications that are well configured and ready to use. Images can be built manually by creating a Dockerfile or can be downloaded from Docker Hub, a public repository that provides many ready-to-use images. 

Docker images are immutable, meaning that once created, images cannot be changed directly. However, images can be created anew by modifying the previous image and giving it a different name. Each image has a name and tags to uniquely identify it. In Docker Hub, the image name usually consists of several parts, such as username, image name, and tag, such as username/name_image:tag. After the image is created, you can use the following command to create an instance of the image in the form of a container.

```
docker run [OPTIONS] IMAGE[:TAG|@DIGEST] [COMMAND] [ARG...]
```

OPTIONS is used for additional configuration for the container, such as naming the container with `--name`. IMAGE is the name of the Image that will be used as the container. TAG is the version of the Image, and defaults to latest if not defined. COMMAND and ARG... are used to specify the commands and arguments that the container will run when it is freshly started.

#### Docker Image Commands

Here are some important commands and their explanations available in **`docker image <COMMAND>`**.

| `<COMMAND>` | Description
| --------- | --------- |
| `build` | This command is used to create a Docker image from a Dockerfile. |
| `history` | Displays the history of changes to an image. |
| `import` | Import an image from a file. The file must contain an image that was previously exported with the **`docker save`** command.
| `inspect` | View details of an image. |
| `load` | Load an image from a saved archive. |
| `ls` | Display a list of downloaded images. |
| `prune` | Delete unused images. |
| `pull` | Download an image from Docker Hub or other registry. |
| `push` | Uploads an image to Docker Hub or other registry. |
| `rename` | Change the name of a downloaded image. |
| `rm` | Delete a downloaded image. |
| `save` | Saves an image into an archive that can be downloaded using the **`docker load`** command |
| `tag` | Tag an image. |

### Docker Container

![docker-container](assets/docker-container.png)

Docker Container can be likened to a box containing a program and all the materials needed for the program to run properly. This box is run separately from the original computer, so that the program in this box can run consistently in various environments without being affected by the configuration and infrastructure that exists on the original computer. 

#### Docker Container Commands

Here are some important commands and their explanations that are available to manage containers in Docker.

| Commands | Description
| --------- | --------- |
| `attach` | Executes a command on the running container. This command will log the user into the container's terminal session. |
| `commit` | Creates a new image of the changes made to the running container. |
| `cp` | Copies files or directories between the host file system and the file system in the container. |
| `create` | Creates a new container, but does not run it. |
| `diff` | Indicates changes to the file system of a running container. |
| `exec` | Executes a command on the running container. |
| `export` | Exports a container into a tar file. |
| `inspect` | View details of a container. |
| `kill` | Forcefully stop a running container. |
| `logs` | View the logs of a container. |
| `ls` | Display a list of running containers. |
| `pause` | Pause a running container. |
| `port` | Displays the ports opened by a container. |
| `prune` | Delete containers that are not running. |
| `rename` | Change the name of a running container. |
| `restart` | Restarts a running container. |
| `rm` | Delete a running container. |
| `run` | Creates a new container and runs it. |
| `start` | Runs a container that has already been created. |
| `stats` | Display CPU, memory, and network information of a running container. |
| `stop` | Stop a running container. |
| `top` | Display the currently running process in a container. |
| `unpause` | Resumes a container that has been paused. |
| `update` | Updates a container with a new configuration. |
| `wait` | Wait for a container to finish executing a command before continuing. |

#### Shell in Docker Container

Since the container environment is separate from the host environment, it is not possible to execute commands in the container using the host shell. To use the shell in Docker Container, you can use the following command.

```
docker exec [OPTIONS] <CONTAINER> <COMMAND>
```

- docker exec is used to execute commands on containers that are already running.
- `<CONTAINER>` is the name or ID of the container to be accessed.
- If you want to execute commands in the container without opening the container shell, you can add `<COMMAND>` to the command to be executed.
- `[OPTIONS]` are the options that can be used.

| `[OPTIONS]` | Description
| -------- | -------- |
| `-d`,`-detach` | Executes the command inside the container in detached mode, so the container runs in the background. |
| `-e`,`--env list` | Set environment variables on the container. |
| `-i`,`--interactive` | Execute commands in interactive mode on the container. |
| `-t`,`--tty` | Allocate a pseudo-TTY (TeleTYpewriter) to the container. |
| `-u`,`--user string` | Specifies the user or UID (user ID) that will be used to execute commands in the container. |
| `-w`,`--workdir string` | Sets the working directory in the container |

![docker-exec](assets/docker-exec.jpg)

#### Dockerfile Example

In this sub-material, an example of Dockerfile implementation for an Nginx web server will be given. Nginx is a web server that can be used as a reverse proxy, load balancer, mail proxy, and HTTP cache, which works by processing incoming requests from clients and sending responses in the form of HTML files or other data.

Here are the contents of the Dockerfile. This Dockerfile will create an image to deploy the **`index.html`** application using Nginx.

```docker
FROM nginx

RUN apt-get update && apt-get upgrade -y

COPY index.html /usr/share/nginx/html

EXPOSE 8080

CMD ["nginx", "-g", "daemon off;"]
```

In the Dockerfile above, the steps are as follows:

- **`FROM nginx`**: Takes the nginx image as the base image to build a new image. This base image will be the foundation for the image created.
- **`RUN apt-get update && apt-get upgrade -y`**: Run the **`update`** and **`upgrade`** commands on the container using the **`apt-get`** package manager. This command will update the package list and upgrade the existing package.
- **`COPY index.html /usr/share/nginx/html`**: Copies the **`index.html`** file from the build context directory (where the Dockerfile is located) into the **`/usr/share/nginx/html`** directory inside the container. This **`index.html`** file will be used by the Nginx web server to display on the web page.
- **`EXPOSE 8080`**: Allows port 8080 to be used by the container. Although this port is allowed, we still need to perform port binding when running the container.
- **`CMD ["nginx", "-g", "daemon off;"]`**: Runs the nginx command inside the container, with the argument **`-g "daemon off;"`**. This argument will turn on Nginx in foreground mode so that we can see Nginx logs in the console. This command will be the default command that is run when the container runs if no other command is given at the time of running the container.

To run this Dockerfile to become a container, do the following steps.

1. To run Nginx on local, install and start nginx with the following command:

```shell
sudo apt install nginx
sudo systemctl start nginx
```
![nginx-conf](assets/start-nginx.jpg)

2. Create a new directory, in that directory create a Dockerfile and **`index.html`** with a folder structure according to [this](./playground/nginx-app/). 

3. In that directory, run the **`docker build -t <image name> .`** command to create a new image from an existing Dockerfile. Fill in the image name as desired.

![Docker-build](assets/docker-build.jpg)

4. Then check on **`docker image ls`**, whether the image that was built is already available.
![Docker image ls](assets/docker-ls.jpg)

5. Then the existing image can be used, with the **`docker run -d -p 8080:80 <image name>`** command to run a container from the image. Check with **`docker ps`** if the container is running.
![Docker-run](assets/run-container.jpg)

6. Visit the result of the running container on **`localhost:8080`** then the 'Welcome to Nginx' website will appear.

### Docker Hub

Docker Hub is a cloud platform that provides a repository for Docker Image. Docker Hub allows users to easily retrieve, store, and distribute Docker images. 

In Docker Hub, users can search for Docker images created by the community or create their own images and share them with others. Docker Hub also allows users to automate build and test images using Dockerfile. With Docker Hub, users can easily manage the Docker images they use in their applications and reduce time and effort in application development, distribution, and deployment.

### Docker Service Advanced

### Docker Compose

Docker Compose is a tool for managing and running applications that consist of one or more containers. Here are some of the things Docker Compose can do:

- Define, configure, and run multiple Docker Containers at once using a simple YAML configuration file.
- Define the Docker Image for each Docker Container, set network settings, determine the required volume, and perform other configurations in a single configuration file.
- Simplify the process of setting up and deploying applications in different production or development environments in a consistent way.

Some important commands for managing Docker Compose and their explanations are available in `docker compose <COMMAND>`.


| `<COMMAND>` | Description
| --- | --- |
| up | Create and start the container according to the configuration in the Docker Compose file.	|
| up -d | Same as docker-compose up, but runs the container in the background (detached mode). So that the docker compose process is not displayed in the terminal.	|
| down | Stop and delete the container generated by docker-compose up.	|
| build | Creates an image for the service defined in the Docker Compose configuration.	|
| start | Run the created container.	|
| stop | Stop the running container.	|
| restart | Stop and restart the container.	|
| ps | Displays the status of the container being run by Docker Compose.	|
| logs | Display the logs of the service run by Docker Compose.	|
| exec | Execute a command in the container.	|
| config Validates and displays the configuration of Docker Compose.	|
| kill | Force stop a running container.	| 

The following is an example of applying Docker Compose to create a web application that consists of three services, namely frontend, backend, and database.

```YAML
version: '3'
services:
  backend:
    build: ./backend
    ports:
      - "8080:8080"
    environment:
      DB_HOST: database
  frontend:
    build: ./frontend
    ports:
      - "3000:3000"
    environment:
      REACT_APP_BACKEND_URL: http://backend:8080
  database:
    image: postgres
    environment:
      POSTGRES_USER: myuser
      POSTGRES_PASSWORD: mypassword
      POSTGRES_DB: mydb
```

The following is an explanation of the configuration above:

| Properties | Description
| --- | --- |
| `version: '3'` | The version of Docker Compose used in the configuration. |
| `services` | The main component that defines the service to be run. In the configuration above, there are 3 services, namely frontend, backend, and database. |
| `backend` | The name of the service to be run. |
| `build: ./backend` | Specifies the directory where Docker will build the image for the backend service. |
| `ports: - "8080:8080"` | Defines the port mapping, where `port 8080` on the container will be forwarded to `port 8080` on the host. |
| `environment: DB_HOST: database` | Defines the environment variable on the service backend, where `DB_HOST` will be set as the database. |
| `frontend` | The name of the service to run. |
| `build: ./frontend` | Specifies the directory where Docker will build the image for the frontend service. |
| `ports: - "3000:3000"` | Defines the port mapping, where `port 3000` on the container will be forwarded to `port 3000` on the host. |
| `environment: REACT_APP_BACKEND_URL: http://backend:8080` | Defines the environment variable on the frontend service, where `REACT_APP_BACKEND_URL` will be set as `http://backend:8080`. |
| `database` | The name of the service to run. |
| image: postgres | Defines the image to be used for the database service. |
| `environment: POSTGRES_USER: myuser POSTGRES_PASSWORD: mypassword POSTGRES_DB: mydb` | Defines the environment variables for the database service, where `POSTGRES_USER` will be set as `myuser`, `POSTGRES_PASSWORD` will be set as `mypassword`, and `POSTGRES_DB` will be set as `mydb`. |

### Docker Data Management

Docker Data Management is a concept for managing data or files in Docker. When running an application or service in a Docker Container, the data generated by the application can be stored in the container itself or in a volume separate from the container.

In Docker, there are several types of mounts or connectors used to manage data, such as volume, bind mount, and tmpfs mount. A developer can choose the right type of mount according to the needs of the application being run in the container. In addition, Docker also provides several commands to manage data on Docker Volume, such as displaying volume information, deleting volumes, and setting volume driver options. By using these commands, developers can manage data in Docker easily and efficiently.

Understanding Docker Data Management is very important to ensure that the data generated by applications running in containers is maintained and not lost when the container is removed or shut down.

### Docker Mount

![docker-mount](assets/docker-mounts.png)

There are several types of Docker Mounts as follows:

- **Volume**
  Docker Volume is a feature in Docker that allows developers to manage the data needed by the container separately from the container itself. Docker Volume allows containers to share data with hosts, other containers, or with data storage services provided by cloud service providers.
- **Bind Mount**
  Bind mount is a type of mount in Docker that allows a file or directory on the host machine to be used by a Docker Container.
- **tmpfs Mount**
  tmpfs mount is a type of mount in Docker that allows to store data temporarily in RAM memory on the host.

## Reference

[Module Pelatihan Docker](https://github.com/arsitektur-jaringan-komputer/Pelatihan-Docker)
