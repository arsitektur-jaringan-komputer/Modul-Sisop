# Module 3 - Operating System Booting

## Prerequisite

- Using **VMware Player**, with nested virtualization enabled.
- Install Linux Ubuntu 22.04 with at least 4GB memory and 4 vCPU.

## Learning Outcomes

- Understand the Linux operating system booting process.
- Understand the basic concepts of the Linux kernel and creating a root filesystem (initramfs).
- Be able to compile the Linux kernel and create a root filesystem using BusyBox.
- Be able to emulate an operating system using QEMU.
- Be able to create a bootable ISO disk and run emulation using the ISO.

## Table of Contents

- [i. Prerequisite](#prerequisite)
- [ii. Learning Outcomes](#learning-outcomes)
- [iii. Table of Contents](#table-of-contents)
- [1. Introduction](#introduction)
  - [1.1 Definition of Booting](#definition-of-booting)
- [2. Linux Kernel](#linux-kernel)
  - [2.1 Definition of Linux Kernel](#definition-of-linux-kernel)
  - [2.2 Preparing the Linux Kernel](#preparing-the-linux-kernel)
- [3. Creating a Root Filesystem](#creating-a-root-filesystem)
  - [3.1 Definition of Root Filesystem](#definition-of-root-filesystem)
  - [3.2 Single User vs Multi User](#single-user-vs-multi-user)
  - [3.3 Single User](#single-user)
  - [3.4 Multi User](#multi-user)
- [4. Emulation with QEMU](#emulation-with-qemu)
  - [4.1 Understanding QEMU](#understanding-qemu)
  - [4.2 Steps for Emulation with QEMU](#steps-for-emulation-with-qemu)
  - [4.3 Testing Inside the Shell](#testing-inside-the-shell)
  - [4.4 Restarting the Boot Process](#restarting-the-boot-process)
- [5. Creating a Bootable ISO Disk](#creating-a-bootable-iso-disk)
- [6. Emulating ISO File Execution](#emulating-iso-file-execution)

## Introduction

<div align="center">
    <img src="https://github.com/user-attachments/assets/4d84c749-9966-485f-9ff4-338979238621" width="800" />
</div>

Imagine you are about to start a car. When you turn the key or press the start button, the engine starts, the electronic systems activate, and within seconds, the car is ready to use. But have you ever wondered what actually happens before the car is ready to drive?

The same thing happens when you turn on a computer or laptop. When you press the power button, the system does not immediately become "ready to use." There is a long process happening behind the scenes, and this is called the booting process.

### Definition of Booting

**Booting** is the process that occurs from the moment a computer is powered on until the operating system is fully active and ready to use. This is a crucial initial stage in the life of an operating system because without a successful booting process, no applications can run.

#### General Booting Stages

<div align="center">
    <img src="https://github.com/user-attachments/assets/26e6edb8-e517-437a-909d-a92ee4bdad5a" width="600" />
</div>

The booting process can be divided into several main stages:

1. **Power-On Self Test (POST)**  
   Once the power button is pressed, the computer's firmware (such as BIOS or UEFI) will perform a POST. This is an initial check to ensure that key components like RAM, processor, and storage devices are functioning properly. If there is an issue, this process will display an error message or emit a beep sound.

2. **Searching for Boot Device & Executing Bootloader**  
   After POST is complete, the firmware will search for a **boot device**, which is the storage medium containing the operating system, based on a configured order (e.g., hard disk, USB, or CD). Within the boot device, the firmware will execute a small program called the **bootloader**, such as GRUB (on Linux) or bootmgr (on Windows). The bootloader is responsible for selecting and loading the operating system kernel into memory.

3. **Loading Kernel**  
   The kernel is the core of the operating system. It is responsible for managing communication between hardware and software, memory, processes, file systems, and more. Once loaded, the kernel will begin initializing all the system's essential components.

4. **Running Init**  
   After the kernel completes initialization, control of the system is handed over to the first process: **init**. This process then starts various other processes, such as network services, login shells, and desktop environments. Init can be a traditional system like SysVinit or a more modern system like systemd.

#### Types of Booting

There are two commonly known types of booting processes:

- **Cold Boot (Hard Boot)**  
   This is the booting process from a completely powered-off state. For example, when you turn on a laptop that is completely off, the process is called a cold boot.

- **Warm Boot (Soft Boot)**  
   This is the rebooting process of a system that is already running. This can be done, for example, by pressing the restart button or using the `reboot` command. Warm booting is often performed after a system update or when there is a minor issue that requires a restart.

#### Importance of the Booting Process and the Role of the Kernel

Without a proper booting process, the operating system **cannot be run**. No matter how advanced or complete the hardware is, without the booting process, a computer is just a pile of components that cannot do anything. The booting process is the **gateway** that opens the way for all operating system functions, from user login, running applications, to managing networks and external devices.

Each stage in the booting process has an important responsibility:

- **Firmware** ensures that the hardware is ready.
- **Bootloader** determines which operating system to run.
- **Kernel** initializes all the essential elements of the system.
- **Init** (the first process) runs the system's basic services.

Failure at any of these stages can cause the system to **fail to boot**, be unable to enter the operating system, or even crash. Therefore, understanding how the booting process works is very important, especially when developing operating systems or troubleshooting.

One of the most vital parts of this process is the **kernel**. The kernel acts as a bridge between hardware and software, managing communication between the processor, memory, input/output devices, and running programs. In Linux-based systems, this kernel is called the **Linux Kernel**.

Due to its central role, the booting process heavily depends on whether the kernel can be loaded and run properly. Therefore, in the next chapter, you will learn how to prepare the Linux kernel as the foundational element of a simple operating system that you will build and emulate yourself.

## Linux Kernel

### Definition of Linux Kernel

<div align="center">
    <img src="https://github.com/user-attachments/assets/0cc4b446-64fd-4f41-b9e2-65c3886b1259" width="400" />
</div>

The Linux Kernel is the core of the Linux operating system. It manages the interaction between hardware and software and handles critical tasks such as memory management, processes, file systems, and input/output devices. The kernel is the first component to run after the bootloader loads it into memory.

One of the advantages of the Linux kernel is its **open-source and modular nature**. Anyone can view, modify, and recompile the kernel according to their needs. This makes it ideal for learning, experimentation, and developing customized operating systems, as you will do in this module.

In practice, we do not need to write a kernel from scratch. We simply take the **official Linux kernel source code**, then compile it into a binary file `bzImage` that can be executed by the bootloader and emulated by QEMU. The kernel compilation process requires several system dependencies and initial configurations to determine the features to include in the compiled kernel.

### Preparing the Linux Kernel

At this stage, we will prepare the Linux kernel to be used for building a minimal operating system using **QEMU**.

QEMU is an open-source emulator that allows us to run operating systems in a virtual environment. With QEMU, we can test the kernel we have built without installing it directly on hardware. Further explanation about QEMU will be discussed in a separate section.

The steps to be performed include updating and installing supporting software, downloading the kernel source code, configuring the kernel as needed, and compiling the kernel to produce the `bzImage` file.

1. Update and Install Supporting Software

   Before starting, ensure the system is updated and all necessary software for this process is installed. We will use several tools such as `qemu`, `build-essential`, `bison`, `flex`, and others.

   ```bash
   sudo apt -y update
   sudo apt -y install qemu-system build-essential bison flex libelf-dev libssl-dev bc grub-common grub-pc libncurses-dev libssl-dev mtools grub-pc-bin xorriso tmux
   ```

2. Prepare the Directory

   Create a directory for this project and navigate to it. All kernel building and configuration processes will be performed within this directory.

   ```bash
   mkdir -p osboot
   cd osboot
   ```

3. Download and Extract the Linux Kernel

   The next step is to download the Linux kernel source code version 6.1.1. After downloading, we will extract it into the `linux-6.1.1` directory.

   ```bash
   wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.1.tar.xz
   tar -xvf linux-6.1.1.tar.xz
   cd linux-6.1.1
   ```

4. Configure the Kernel

   The Linux kernel needs to be configured before being compiled. We will start by using a minimal configuration with `make tinyconfig`. After that, we can further customize the configuration using `make menuconfig` to enable features required by the operating system we are building. Some important settings to enable include support for virtualization, file systems, device drivers, and networking.

   ```
   64-Bit Kernel
   General Setup > Configure standard kernel features > Enable support for printk
   General Setup > Configure standard kernel features > Enable futex support
   General Setup > Initial RAM filesystem and RAM disk (initramfs/initrd) support
   General Setup > Control Group Support
   Enable the block layer > Legacy autoloading support
   Enable the block layer > Partition type > Advanced Partition Selection
   Device Drivers > Character devices > Enable TTY
   Device Drivers > Character devices > Virtio console
   Device Drivers > Character devices > /dev/mem virtual device support
   Device Drivers > Network device support > Virtio Network Driver
   Device Drivers > Serial Ata / Paralel ATA
   Device Drivers > Block Devices > Virtio block driver
   Device Drivers > Block Devices > loopback device support
   Device Drivers > Block Devices > RAM block device support
   Device Drivers > Virtio drivers
   Device Drivers > Virtualization Drivers
   Device Drivers > Generic  Driver Options > Maintain a devtmpfs at filesystems
   Device Drivers > Generic Driver Options > Automount devtmpfs at /dev
   Executable file formats > Kernel Support for ELF binaries
   Executable file formats > Kernel Support scripts starting with #!
   File Systems > FUSE support
   File Systems > The extended 3 filesystem
   File Systems > The extended 4 filesystem
   File Systems > Second extended fs support
   File Systems > Virtio Filesystem
   File Systems > Kernel automounter support
   File Systems > Pseudo File Systems > /proc file system support
   File Systems > Pseudo File Systems > sysctl support
   File Systems > Pseudo File Systems > sysfs file system support
   Networking Support > Networking options > Unix domain sockets
   Networking Support > Networking options > TCP/IP Networking
   ```

   This configuration ensures that the resulting kernel can work well in a virtual environment and supports the basic operations required.

   ```bash
   make tinyconfig
   make menuconfig
   ```

   After completing the configuration, you can save the `.config` file and proceed to the next step.

5. Compile the Kernel

   With the configuration ready, we can now start the kernel compilation process. This process will produce the `bzImage` file, which is the kernel file ready for booting.

   ```bash
   make -j$(nproc)
   ```

   The command above compiles the kernel using all available CPU cores on the system.

6. Generate the `bzImage` File

   After the compilation is complete, we will obtain the `bzImage` file in the `arch/x86/boot/` directory. Move this file to the `osboot` directory in preparation for the next step, which is creating the root filesystem and emulating it using QEMU.

   ```bash
   cp arch/x86/boot/bzImage ..
   ```

## Creating a Root Filesystem

### Definition of Root Filesystem

<div align="center">
    <img src="https://github.com/user-attachments/assets/8c57f20e-f95e-45f4-a4d3-7a5b357324c5" width="800" />
</div>

Imagine an operating system as a house. The **root filesystem** is the _foundation and basic framework of that house_. All rooms (directories like `/bin`, `/etc`, `/dev`, etc.) are built on top of this rootfs, and all activities of the inhabitants (applications and services) depend on it for the house to function properly. Without rootfs, the house is like a structure without an entrance, tools, or rules.

#### What is a Root Filesystem?

- The **Root Filesystem** is the file structure used by the operating system to store essential files required to run the system, such as configurations, hardware, and basic applications.
- It is the **first filesystem loaded** after the booting process begins and serves as the central storage for all critical files needed by the system.

#### Components in a Root Filesystem

The root filesystem typically contains several important directories, including:

- **`/bin`** – Stores basic applications and programs used to run the system.
- **`/etc`** – Stores system and application configuration files.
- **`/lib`** – Contains system libraries required by programs to run.
- **`/dev`** – Stores device files needed to access hardware.
- **`/home`** – Directory for storing user data.

#### Initial RAM Filesystem (initramfs)

Before the main root filesystem can be mounted, the system requires a temporary filesystem called **initramfs** to start the operating system. Initramfs is a **lightweight filesystem image** loaded into memory during the initial booting phase.

**Functions of Initramfs**:

- Provides basic tools and scripts to prepare the system.
- Configures hardware and checks storage devices.
- Supplies the kernel with the information needed to mount the main root filesystem.

#### BusyBox

To simplify tasks within initramfs, **BusyBox** is used as a collection of lightweight yet comprehensive tools. BusyBox provides various **system utilities** used to configure hardware, manipulate the filesystem, and perform other critical tasks during the booting process.

**Functions of BusyBox**:

- Replaces many standard Linux commands in the operating system with lighter and simpler versions.
- Functions in resource-constrained environments (such as embedded systems or the early booting stage).
- Provides utilities like `ls`, `cp`, `mv`, `mount`, and many others, which are used by initramfs to perform various tasks.

**Installing BusyBox**

BusyBox can be installed using the following command:

```bash
sudo apt install -y busybox-static
```

You can check if BusyBox is installed with the command:

```bash
whereis busybox
```

Typically, **BusyBox** will be located in the `/usr/bin/busybox` directory.

### Single User vs Multi User

The Linux operating system can run in two main modes based on user needs and system complexity: **Single User** and **Multi User**. Both use the root filesystem as the system's foundation but have different approaches, structures, and booting processes.

The **Single User** mode is suitable for system maintenance or debugging as it only allows one user (usually root) to access the system. Meanwhile, the **Multi User** mode is designed to allow multiple users to log in and work simultaneously, with access rights, accounts, and system services management.

#### Comparison: Single User vs Multi User

1. From a Functional Perspective

   | Aspect          | Single User                                                                 | Multi User                                                                         |
   | --------------- | --------------------------------------------------------------------------- | ---------------------------------------------------------------------------------- |
   | **Purpose**     | Used for maintenance, debugging, or minimal systems.                        | Used for daily use, servers, or systems with multiple users.                       |
   | **User Access** | Only one user (usually root) can log in.                                    | Multiple users can log in simultaneously with different access rights.             |
   | **Interaction** | No interaction between users, focused on basic system tasks.                | Supports interaction between users and parallel processes.                         |
   | **Security**    | Minimal security settings as only root has access.                          | Requires authentication systems, user management, and access rights for each user. |
   | **Use Case**    | Practical for recovery, boot testing, or very lightweight embedded systems. | Common in desktop systems, servers, or work environments involving multiple users. |

2. From a System Creation Perspective

   **Single User**

   - **Root Filesystem Structure:**
     - Minimal: `/bin`, `/dev`, `/proc`, `/sys`, and the `init` file.
     - No user directories (`/home`, `/etc/passwd`, etc.).
   - **`init` File:**
     - Simple. Usually contains:
       ```sh
       #!/bin/sh
       echo "Init started"
       exec /bin/sh
       ```
     - After mounting, directly enters the shell as root without a login process.
   - **User Management:** Not required, as only root runs the shell.
   - **Booting Process:** Fast and simple, no login daemon or access rights management.

   **Multi User**

   - **Root Filesystem Structure:**
     - More complete: Includes `/etc/passwd`, `/etc/group`, `/etc/shadow`, `/home/user1`, etc.
     - Uses BusyBox for `login`, `adduser`, `getty`, and other system management tools.
   - **`init` File:**
     - More complex, as it must run the login process via `getty`:
       ```sh
       #!/bin/sh
       mount -t proc none /proc
       mount -t sysfs none /sys
       mount -t devtmpfs none /dev
       /bin/hostname multiuser
       setsid /bin/getty -n -l /bin/login 115200 tty1 &
       exec /bin/sh
       ```
     - Login process active on `tty1`, and can be added for `tty2`, `tty3`, etc.
   - **User Management:**
     - Users and groups must be created using `adduser` or manually editing `/etc/passwd`.
     - Passwords can be encrypted or left blank depending on BusyBox configuration.
   - **Booting Process:** The system runs a login prompt on multiple terminals (multi-console). After logging in, each user gets their own shell according to their access rights.

3. Summary of Differences

   | Aspect                     | Single User                              | Multi User                                            |
   | -------------------------- | ---------------------------------------- | ----------------------------------------------------- |
   | **Main Purpose**           | Maintenance / minimal system             | System ready for use by multiple users                |
   | **Access**                 | Root only                                | Multiple users with login                             |
   | **Filesystem Complexity**  | Simple                                   | Complete with user configuration                      |
   | **`init` File**            | Direct shell                             | Login process via getty                               |
   | **Additional Directories** | No need for `/etc/passwd`, `/home`, etc. | Must include directories and user configuration files |
   | **User Management**        | None                                     | Requires authentication system                        |
   | **Use Case Example**       | Rescue mode, embedded systems            | Server, multi-user workstation                        |

Conclusion

The main difference between **single user** and **multi user** lies not only in the number of users who can log in but also in the level of system complexity. The **single user** mode offers simplicity and speed, ideal for emergencies or lightweight embedded systems. Meanwhile, the **multi user** mode requires additional configuration but provides greater flexibility and scalability, suitable for production systems.

### Single User

After preparing the kernel and root filesystem, we will proceed to create a simpler filesystem for the initial booting stage, known as Single User Mode. In **Single User** mode, the prepared root filesystem will run with only one user who has full control over the system. This mode is useful in the early stages of booting or for simpler systems, such as embedded systems, where only one user is needed to configure or manage the system. Below is a further explanation of how the root filesystem works in single user mode, along with steps to create this root filesystem.

#### Concept of Root Filesystem in Single User Mode

1. **Role of the `init` Program**

   - After the Linux kernel starts the booting process, the first step is to run the **init** program. This **init** program is the first program executed by the kernel with Process ID (PID) 1.
   - **init** is responsible for setting up the environment needed by the system and starting other processes in the correct order.
   - The root filesystem initially loaded by the kernel will include the **init** program, which is usually located in the root directory (`/`).

2. **Using Initramfs in Single User Mode**

   - To simplify the booting process, the root filesystem can be loaded using **initramfs**. Initramfs is a **temporary filesystem** loaded into memory and serves to prepare the system before the main root filesystem is mounted.
   - In this case, **initramfs** is used to configure the system and provide minimal tools, such as BusyBox, to run the **init** program in single user mode.

3. **Why Use BusyBox**

   - In more complex operating systems, the root filesystem is usually stored on permanent storage such as a hard drive or SSD. However, during the initial booting process or on resource-constrained systems, we use **BusyBox**.
   - **BusyBox** is a collection of Unix/Linux utilities packaged into a single executable file. It enables a lighter system by providing various essential commands like `ls`, `cp`, `mount`, and `sh`, which are used to configure and manage the filesystem.

#### Steps to Create a Root Filesystem for Single User Mode

The steps to create a root filesystem using initramfs and BusyBox to provide a minimal yet functional system environment are as follows:

1. **Enter Superuser Mode**

   To make changes to the system, we will work in superuser (root) mode. Use the following command to enter the shell as root:

   ```bash
   sudo bash
   ```

2. **Create a Directory for Initramfs**

   Next, create a `myramdisk` directory and several subdirectories for the filesystem:

   ```bash
   mkdir -p myramdisk/{bin,dev,proc,sys}
   ```

   The directory structure will look like this:

   ```
   myramdisk/
       ├── bin/
       ├── dev/
       ├── proc/
       └── sys/
   ```

3. **Copy Device Files to the `dev` Directory**

   The `/dev` directory in Linux contains important device files like `null`, `tty`, `zero`, and `console`. We will copy them from the host system into the `dev` directory in `myramdisk`.

   ```bash
   cp -a /dev/null myramdisk/dev
   cp -a /dev/tty* myramdisk/dev
   cp -a /dev/zero myramdisk/dev
   cp -a /dev/console myramdisk/dev
   ```

4. **Copy BusyBox to the `bin` Directory**

   Next, copy the **BusyBox** file into the `bin` directory created inside `myramdisk`. Then, install all utilities provided by BusyBox:

   ```bash
   cp /usr/bin/busybox myramdisk/bin
   cd myramdisk/bin
   ./busybox --install .
   ```

   This command will add various Unix/Linux commands like `ls`, `cp`, `mv`, `mount`, and many others to the `bin` directory.

5. **Create the `init` File**

   Inside the `myramdisk` directory, create an **init** file that will be executed first during booting. Populate this file with instructions to mount the filesystem and start an interactive shell.

   ```bash
   #!/bin/sh
   /bin/mount -t proc none /proc
   /bin/mount -t sysfs none /sys
   exec /bin/sh
   ```

   This file will mount `proc` and `sysfs`, enabling the system to communicate with the kernel. After that, it will start the `/bin/sh` shell for user interaction.

6. **Grant Execution Permission to the `init` File**

   To make the `init` file executable, grant it execution permission using the command:

   ```bash
   chmod +x init
   ```

7. **Compress and Create the `initramfs` File**

   After preparing all the necessary files, the next step is to package them using the `cpio` command and compress them with `gzip` to create the initramfs image.

   ```bash
   find . | cpio -oHnewc | gzip > ../myramdisk.gz
   ```

   This command will produce the **myramdisk.gz** file, which contains the minimal filesystem for the initial booting stage.

With the above steps, we have successfully created a root filesystem in single user mode using BusyBox and initramfs.

### Multi User

After successfully running the system in **single user** mode, the next step is to build the system in **multi user** mode using **initramfs**. In this mode, more than one user can log in and use the system simultaneously, making it suitable for general use that requires account and access management.

<div align="center">
    <img src="https://github.com/user-attachments/assets/6d76c080-a33f-4695-9a7c-691181f17d84" width="400" />
</div>

To support multi user, we will construct a more complete root filesystem. We use **BusyBox** as the provider of basic Linux utilities and **initramfs** as the root filesystem loaded into memory during booting. Unlike single user mode, which only provides a direct shell for root, this mode requires a login process on the terminal, necessitating files like `/etc/passwd`, `/etc/group`, and services like `getty`.

#### Steps to Create a Root Filesystem for Multi User Mode

1.  **Enter Privileged Mode (Superuser)**

    To make changes to the system, we will work in **superuser** (root) mode using the command:

    ```bash
    sudo bash
    ```

2.  **Create a Directory for Initramfs**

    Create a directory named `myramdisk` and several subdirectories required by the root filesystem.

    ```bash
    mkdir -p myramdisk/{bin,dev,proc,sys,etc,root,home/user1}
    ```

3.  **Copy Device Files to the `dev` Directory**

    The `/dev` directory contains important devices like `null`, `tty`, `zero`, and `console`. We will copy these device files from the host system into the `dev` directory in `myramdisk`.

    ```bash
    cp -a /dev/null myramdisk/dev
    cp -a /dev/tty* myramdisk/dev
    cp -a /dev/zero myramdisk/dev
    cp -a /dev/console myramdisk/dev
    ```

4.  **Copy BusyBox to the `bin` Directory**

    Copy the **BusyBox** file into the `bin` directory and run the BusyBox utility installation:

    ```bash
    cp /usr/bin/busybox myramdisk/bin
    cd myramdisk/bin
    ./busybox --install .
    ```

5.  **Create Root Password**

    Use the `openssl` command to create an encrypted root password:

    ```bash
    openssl passwd -1 myrootpassword
    ```

    Copy the output of the above command, as it will be used to create the user and password database.

6.  **Create the `passwd` File in the `etc` Directory**

    Create a `passwd` file in the `etc` directory to store information about the root account and the user we will create (e.g., `user1`):

    ```bash
    root:<<generatedrootpassword>>:0:0:root:/root:/bin/sh
    user1:<<generateduserpassword>>:1001:100:user1:/home/user1:/bin/sh
    ```

7.  **Create the `group` File in the `etc` Directory**

    Create a `group` file in the `etc` directory containing group settings for the newly created user:

    ```bash
    root:x:0:
    bin:x:1:root
    sys:x:2:root
    tty:x:5:root,user1
    disk:x:6:root
    wheel:x:10:root,user1
    users:x:100:user1
    ```

8.  **Create the `init` File**

    Return to the `myramdisk` directory and create an **init** file that will be called by the bootloader during system booting. Populate the `init` file with instructions to mount the filesystem and run the login process:

    ```bash
    #!/bin/sh
    /bin/mount -t proc none /proc
    /bin/mount -t sysfs none /sys

    while true
    do
            /bin/getty -L tty1 115200 vt100
            sleep 1
    done
    ```

    This file will mount `proc` and `sysfs`, enabling communication between user space and the kernel. Then, the `getty` command will wait for login input from users via the console.

9.  **Grant Execution Permission to the `init` File**

    To make the `init` file executable, grant it execution permission:

    ```bash
    chmod +x init
    ```

10. **Compress and Create the `initramfs` File**

        After preparing all the necessary files, we will package them using the **cpio** utility and then compress them with **gzip** to create the **initramfs** image.

        ```bash
        find . | cpio -oHnewc | gzip > ../myramdisk.gz
        ```

        This command will produce the **myramdisk.gz** file, which contains the minimal filesystem for booting.

With the above steps, we have successfully created a root filesystem with multi-user capabilities using initramfs and BusyBox. This system allows multiple users to log in, with one main user (root) and one additional user (user1). When the system boots, users can log in using the credentials we have defined.

## Emulation with QEMU

### Understanding QEMU

**QEMU** (Quick Emulator) is a program used to run **operating systems** or **applications** in a **virtual environment** (similar to another computer) without requiring different physical hardware. With QEMU, you can run different operating systems, such as Linux on Windows, or try entirely different systems, like ARM or RISC-V, on an x86-based machine (a regular computer).

Imagine you want to try a different system, for example, testing **Windows 10** on a **Mac**. Instead of installing Windows directly on the Mac's hard drive (which can be time-consuming and risky), you can use QEMU to **"emulate"** a Windows machine inside the Mac, allowing you to test Windows 10 without changing anything on the original computer.

QEMU works by **emulating** (mimicking) components in a computer such as the **CPU** (processor), **memory**, and **hardware**. This allows you to run operating systems or applications that would normally only work on specific hardware, without needing the corresponding physical machine.

#### Difference Between QEMU and Virtual Machines (VM)

**QEMU** and **Virtual Machines (VM)** are often used for the same purpose, which is running different operating systems, but they differ in how they work.

1. **Virtual Machine (VM)**

   - **What is a VM?** A VM is a **virtual machine** that runs on top of the main operating system. With a VM, you create a **virtual machine** that functions like a separate physical computer. You can run an operating system (e.g., Linux) on top of another operating system (e.g., Windows) using programs like **VirtualBox** or **VMware**.
   - **How a VM Works:** A virtual machine uses **virtualization** to divide the physical computer's resources (like CPU, RAM) into multiple virtual machines. So, even though all virtual machines share the same physical computer, they work as if they have their own separate computer.

2. **QEMU**

   - **What is QEMU?** QEMU is a **flexible emulator**. QEMU can perform **full emulation** of a computer, meaning it can mimic **hardware** (like CPUs and other devices) from another machine. This allows you to run operating systems designed for different hardware (e.g., ARM, MIPS) on top of a different machine (e.g., x86).
   - **How QEMU Works:** QEMU not only supports **virtualization** but also **emulation**. This means QEMU can "mimic" hardware entirely, allowing you to run various types of operating systems without requiring the corresponding physical machine.

3. **Comparison Between QEMU and VM**

   | **Aspect**           | **QEMU**                                                                 | **VM**                                                                                      |
   | -------------------- | ------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------- |
   | **Technology Type**  | Emulation (mimics hardware)                                              | Virtualization (shares physical resources)                                                  |
   | **Main Function**    | Runs operating systems from different architectures                      | Runs multiple operating systems on one physical machine                                     |
   | **Use Case Example** | Running operating systems for different architectures (e.g., ARM on x86) | Running multiple operating systems on one machine (e.g., Windows and Linux on one computer) |

### Steps for Emulation with QEMU

After creating the `bzImage` (kernel) and `myramdisk.gz` (initramfs) files, you can run the operating system you built using **QEMU**. Here are the steps to follow:

1. Navigate to the `osboot` Directory

   First, go to the directory where you saved the build files:

   ```bash
   cd osboot
   ```

   This directory should contain the following files:

   - `bzImage` → the compiled kernel
   - `myramdisk.gz` → the root filesystem you created

2. Run the Emulation with QEMU

   Use the following command to run QEMU with specific configurations:

   ```bash
   qemu-system-x86_64 \
   -smp 2 \
   -m 256 \
   -display curses \
   -vga std \
   -kernel bzImage \
   -initrd myramdisk.gz
   ```

   Explanation of Options:

   | Option                 | Function                                                         |
   | ---------------------- | ---------------------------------------------------------------- |
   | `-smp 2`               | Provides 2 vCPUs (virtual CPUs) for the emulated system          |
   | `-m 256`               | Allocates 256 MB of memory                                       |
   | `-display curses`      | Displays output in text mode (can be used in a regular terminal) |
   | `-vga std`             | Sets standard VGA display (80x25), sufficient for text display   |
   | `-kernel bzImage`      | Specifies the kernel to use for booting                          |
   | `-initrd myramdisk.gz` | Specifies the root filesystem (initramfs) to load                |

After running the above command, QEMU will start the booting process. If there are no errors, you will enter the BusyBox shell (if using an `init` script that launches a shell) or see a login process if using a multi-user configuration.

### Testing Inside the Shell

Once the system successfully boots through QEMU, you will see a shell or login prompt depending on whether you are using **single user** or **multi-user** mode.

#### **Single User**

You will directly enter the `sh` shell provided by **BusyBox**. This means the system only provides direct access to the terminal without a login process.

Example display:

```
/ #
```

Here, you can try some basic Linux commands like:

```sh
ls
cat /proc/cpuinfo
echo "Hello from QEMU!"
```

The goal is to ensure that:

- The file system is successfully loaded
- BusyBox is functioning correctly
- The system responds to input

#### **Multi User**

In this mode, you will see a login prompt. The system will ask you to log in using the username and password you configured earlier (e.g., in the `/etc/passwd` and `/etc/shadow` files).

Example display:

```
Welcome to Mini Linux
myos login: user1
Password:
```

After successfully logging in, you will enter the shell and can perform tests such as:

```sh
whoami
ls /home
```

Things to test:

- The login process works correctly
- User permissions are appropriate (cannot access root directories without permission)
- You can navigate directories and execute basic commands

### Restarting the Boot Process

If you want to **restart the boot process** of the system you emulated with QEMU, you need to stop the QEMU process first, then run it again as before.

1. **Stop the QEMU Process**

   Use the following command to stop the QEMU process:

   ```sh
   pkill -f qemu
   ```

   This command will find processes containing the word "qemu" and terminate them. Ensure you are not running any other important processes with the same name.

2. **Run the System Again Using QEMU**

   Repeat the same QEMU command as before:

   ```sh
   qemu-system-x86_64 \
     -smp 2 \
     -m 256 \
     -display curses \
     -vga std \
     -kernel bzImage \
     -initrd myramdisk.gz
   ```

This will reboot the system from the beginning with the same settings, allowing you to test again whether the system runs stably and as expected.

## Creating a Bootable ISO Disk

A **bootable ISO disk** is a `.iso` file that contains a complete file system structure of an operating system and can be used for **booting**. This file can be run through emulators like QEMU, burned to a CD/DVD, or written to a USB drive to run the system directly (live system).

Creating an ISO is useful for:

- **Distributing a custom operating system**
- **Quick testing and deployment** to other machines
- Building a **custom minimal Linux live system**

To make the ISO bootable, we need a **bootloader**, which is the initial program executed when the computer starts. Here, we will use **GRUB** (GRand Unified Bootloader) as the bootloader and combine it with the kernel (`bzImage`) and root filesystem (`myramdisk.gz`) into a single ISO using the `grub-mkrescue` tool.

**Steps:**

1. **Navigate to the `osboot` directory**:

   ```bash
   cd osboot
   ```

2. **Create the ISO directory structure**:

   ```bash
   mkdir -p mylinuxiso/boot/grub
   ```

3. **Copy the kernel and root filesystem files**:

   ```bash
   cp bzImage mylinuxiso/boot
   cp myramdisk.gz mylinuxiso/boot
   ```

4. **Create the GRUB configuration file**:
   Create a `grub.cfg` file in `mylinuxiso/boot/grub` with the following content:

   ```cfg
   set timeout=5
   set default=0

   menuentry "MyLinux" {
     linux /boot/bzImage
     initrd /boot/myramdisk.gz
   }
   ```

   > This file creates a GRUB menu displaying a boot option named "MyLinux" and directs the system to use the provided kernel and initrd.

5. **Generate the bootable ISO file**:
   Run the following command from the `osboot` directory:
   ```bash
   grub-mkrescue -o mylinux.iso mylinuxiso
   ```

## Emulating ISO File Execution

After successfully creating the **bootable ISO file**, the next step is to **test the ISO using an emulator**. Here, we will use **QEMU** to run the system we built, simulating booting from a virtual CD-ROM.

This emulation allows us to:

- Verify that the bootable ISO system works correctly
- Run the BusyBox shell from the ISO
- Check the integration of the kernel and initramfs

**Steps for Emulation:**

1. **Navigate to the `osboot` directory**:

   ```bash
   cd osboot
   ```

2. **Run the system with QEMU using the ISO file**:

   ```bash
   qemu-system-x86_64 \
     -smp 2 \
     -m 256 \
     -display curses \
     -vga std \
     -cdrom mylinux.iso
   ```

   **Explanation of parameters**:

   - `-smp 2` → uses 2 virtual CPUs
   - `-m 256` → allocates 256 MB of RAM
   - `-display curses` → displays output in the terminal (text mode)
   - `-vga std` → standard VGA for 80x25 resolution display
   - `-cdrom mylinux.iso` → specifies the ISO file to boot

**Results**

- The screen will display the booting process
- A **GRUB** menu will appear with the option: `MyLinux`
- After selecting, the system will load the kernel and `initrd`, then enter the **BusyBox shell**

**Testing in BusyBox Shell**

Once inside the shell, try the following commands to test the system environment:

```bash
ls -la
ls
whoami
ps
```

These commands will:

- Display the directory contents (`ls`)
- Show the active user (`whoami`)
- List running processes (`ps`)

**Restarting the Emulation**

If you want to rerun the emulation:

1. Stop QEMU with:

   ```bash
   pkill -f qemu
   ```

2. Run the `qemu-system-x86_64` command again as shown above.
