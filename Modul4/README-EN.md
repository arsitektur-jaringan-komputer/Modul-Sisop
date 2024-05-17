# Modul 4 - FUSE and Simple Kernel

## Learning Outcomes

1. Understand the file system on the operating system
2. Can create a simple FUSE program
3. Understand the steps of creating an operating system

## Table of Contents

- [i. Learning Outcomes](#learning-outcomes)
- [ii. Table of Contents](#table-of-contents)
- [1. File System](#file-system)
  - [1.1. Types of File System](#types-of-file-system)
  - [1.2. Virtual File System](#virtual-file-system)
  - [1.3. Dentry](#dentry)
  - [1.4. Superblock](#superblock)
  - [1.5. Inode](#inode)
- [2. File System in Userspace](#file-system-in-userspace)
  - [2.1. Definition of FUSE](#definition-of-fuse)
  - [2.2. FUSE Installation](#fuse-installation)
  - [2.3. How FUSE Works](#how-fuse-works)
  - [2.4. Making a FUSE Program](#making-a-fuse-program)
  - [2.5. Unmount FUSE](#unmount-fuse)
- [3. Operating System](#operating-system)
  - [3.1 What is Operating System?](#what-is-operating-system)
  - [3.2 What is Kernel?](#what-is-kernel)
  - [3.3 What is Shell?](#what-is-shell)
  - [3.4 Operating System Booting Process](#operating-system-booting-process)
  - [3.5 Interrupts](#interrupts)
- [4. Simple OS Development](#simple-os-development)
  - [4.1 Tools Installation](#tools-installation)
  - [4.2 Preparing Disk Image](#preparing-disk-image)
  - [4.3 Make Simple Bootloader](#make-simple-bootloader)
  - [4.4 Running Simple Bootloader](#running-simple-bootloader)
  - [4.5 Make Assembly Kernel](#make-assembly-kernel)
  - [4.6 Make C Kernel](#make-c-kernel)
  - [4.7 Linking Assembly and C Kernel](#linking-assembly-and-c-kernel)
  - [4.8 Extending Bootloader](#extending-bootloader)
  - [4.9 Putting Bootloader and Kernel into Disk Image](#putting-bootloader-and-kernel-into-disk-image)
  - [4.10 Running Simple OS](#running-simple-os)
  - [4.11 Automate Build Process](#automate-build-process)

# File System

_File system_ is a logical structure used to control data access such as how it is stored or retrieved. _File system_ itself has many types with the use of different algorithms. Each Operating System (OS) has a different file system support. The file system is used to organize and store files on a storage device.

The _File system_ provides a way to separate the data on the drive into a single shape, namely files. _File system_ also provides a way to store data on files, for example **filename**, **permissions**, and other attributes. In a File System, there also exists an **index** which contains a list of files that exists in a storage location, so that the Operating System can see what is in the storage location.

![enter image description here](./assets/linux-filesystem.png?raw=true)

### Types of File System

**1. File System Disk**

_File system disk_ is a _file system_ designed to store data in a data storage media. Example: FAT (FAT 12, FAT 16, FAT 320), NTFS, HFS, HFS+, ext2, ext3, ext4, ISO 9660, ODS-5 dan UDF.

- FAT32 and NTFS is a File System di Windows.

![enter image description here](./assets/xformat-file-system-in-windows-disk-utility.png.pagespeed.gp+jp+jw+pj+ws+js+rj+rp+rw+ri+cp+md.ic._Lfdba02dY.png?raw=true)

- Ext2, Ext3, Ext4 is a File Sytem in Linux.

![enter image description here](./assets/648x463xgparted-showing-primary-extended-and-logical-partitions.png.pagespeed.ic.foZUIWymSb.png?raw=true)

- APFS, HFS dan HFS+ is a File System from MacOS

![enter image description here](./assets/xformat-drive.png.pagespeed.gp+jp+jw+pj+ws+js+rj+rp+rw+ri+cp+md.ic.GnI_H55kwh.png?raw=true)

**2. File System Flash**

_File system flash_ is a _file system_ yang didesain untuk menyimpan data pada media _flash memory_. Hal ini menjadi lazim ketika jumlah perangkat mobile semakin banyak dan kapasitas _memory flash_ yang semakin besar. Contohnya pada linux flash filesystems yaitu JFFS, JFFS2, YAFFS, UBIFS, LogFS, F2FS.

**3. File System Database**

A new concept for file management is the concept of database-based file system. As an improvement to hierarchical management structure, files are identified by their characteristics, such as _file_ type, topic, author, or the same metadata.

![enter image description here](./assets/GUID-5E2B22F9-F87A-4A95-BD1B-AAC76C9F8462-default.gif?raw=true)

![enter image description here](./assets/kdbfs.png?raw=true)

**4. File System Transaksional**

Some programs sometimes require changes to multiple files. If a process fails, changes will be reverted. An example is when installing a software which runs the writing process of several files. If an error occurs during the writing process, and the software is left **half installed**, then the **half installed** software will be damaged or unstable.

Transactional File System won't let that happen. This file system guarantees that if an error occurs, the process will be canceled, and the files that have been created during the process will be rolled back to normal. Examples of this File System on UNIX are the Valor File System, Amino, LFS and TFFS,

**5. Network File System**

Network _File system_ is a _file system_ which acts as a client for the remote file access protocol, granting access to _files_ on a _server_. Examples of these network _file system_ are NFS, AFS, SMB protocol clients, and FTP and WebDAV clients.

**6. Journaling File System**

_File system_ that records every change that occurs on the storage device into a journal (usually a circular log in a certain area) before making changes to the _file system_. Such system files are less likely to be damaged when a power failure or system crash occurs.

### Virtual File System

A virtual file system (VFS) is a layer of software in the kernel that provides a _file system interface_ for the program's _user space_. _Virtual file system_ functions so that various types of _file system_ can be accessed by computer applications in a uniform manner. VFS provides an interface between the _system call_ and the real system.

### Dentry

Dentry or **Directory Entry** is a data structure that translates filenames into their inodes. Examples of information stored in dentry are _name_, _pointer to inode_, _pointer to parent dentry_, _use count_, and others. There are also commands in VFS dentry are D_compare, D_delete, D_release.

![enter image description here](./assets/figure5.gif?raw=true)

### Superblock

Every file system that is mounted will be represented by a VFS Superblock. _Superblock_ is used to store information about the partition. _Superblock_ stores the following information:

- _Device_: Is the _device identifier_, for example **/ dev / hda1** is the first _harddisk_ on the system having device identifier **0 × 300**.

- _Inode Pointer_: Is a _pointer_ which points to the first inode of the filesystem.

- _Blocksize_: Indicates the size of a _block_ file system, for example **1024** **bytes**.

- _Superblock Operation_: Is a pointer to a set of superblock routines (functions) from the _file system_, for example **read**, **write**, and so on.

- _File System Type_: Shows the type of _file system_, for example: EXT2, FAT, NTFS.

- _File System Specific_: It is a pointer to the information required by the \_file system.

![enter image description here](./assets/figure3.gif?raw=true)

### Inode

An inode is a VFS abstraction for files. Each file, directory, and other data on VFS is represented by one and only one VFS inode. VFS inodes exist only in kernel memory and are stored in the inode cache as long as they are needed by the system. The information stored by the VFS Inode includes:

- _Device_: Shows the device identifier of a device that stores files or a _directory_.

- _Inode Number_: Is a unique inode number in the _file system_.

- Mode: Describes what the VFS inode represents.

- _User_ ID: Is an identifier for the owner of the file.

- _Time_: Indicate when the creation, modification, and writing of a file.

- _Blocksize_: Shows the size of the _block_ used by the file.

- _Inode Operations_: Is _pointer_ to a _routine_ who perform various operations on inode.

- _Count_: Shows the number of times a system has used an inode.

- _Lock_: Used to lock the VFS inode.

- _File System Specific Information_: Indicates specific information required by an inode.

![enter image description here](./assets/figure4.gif?raw=true)

Following is the relationship between dentry, superblock, and inode on a Virtual File System.

![enter image description here](./assets/daHCZ.gif?raw=true)

![enter image description here](./assets/dentry.JPG?raw=true)

## File System in Userspace

### Definition of FUSE

FUSE (Filesystem in Userspace) is an interface where we could make our own file system inside a userspace on linux.

The advantage of using FUSE is that we can use any available library to create your own file system without having to deeply understand what the file system is actually doing in the kernel space. This is done because the FUSE module can bridge the file system code in the userspace with the file system in the kernel space. Some of the other benefits of FUSE are as follows:

- Can be loaded and installed by ordinary users, for network access, to get archived files, for removable media, etc.
- If the FUSE system driver crashes, it will not affect the kernel.
- FUSE can be deployed quickly, both because it does not require administrator intervention to install and because it can be easily accessed by any supported OS.
- No license issues related to static connection with the kernel.

![Fuse Illustration](./assets/fuse.png?raw=true)

One interesting example of FUSE is [GDFS][7bb7b7cc](Google Drive File System), where this GDFS allows us to mount our Google Drive to a linux system and use it like a regular linux file.

[7bb7b7cc]: https://github.com/robin-thomas/GDFS "GDFS"

![enter image description here](./assets/google-drive-2.png?raw=true)

To implement FUSE, we must create a program that is linked to the library `libfuse`. The purpose of this program is to specify how the file system responds to read/write/stat of a request and to mount the original file system (kernel space) to the file system new (userspace). So when a user deals with read/write/stat requests in the file system (userspace), the kernel will forward the input and output of the request to the FUSE program and the program will respond back to the user.

For more details, let's try to create a FUSE program.

### FUSE installation

First of all we have to make sure that FUSE is installed on your device.

```
$ sudo apt update
$ sudo apt install libfuse*
```

### How FUSE Works

- `fuse_main()` (lib/helper.c) = as the main (userspace) function, the user program calls the fuse_main() function then the fuse_mount() function is called.
- `fuse_mount()` (lib/mount.c) = creates a UNIX domain socket, then forks and creates a child process that runs fusermount
- `fusermount()` (util/fusermount.c) = to check whether the FUSE module has been loaded. Then open /dev/fuse and send the handling file via the UNIX domain socket back to the fuse_mount() function.
- `fuse_new()` (lib/fuse.c) = creates a data structure containing the space used to store the file system data
- `fuse_loop()` (lib/fuse.c) = read file system calls from /dev/fuse

These are some of the functions provided by **FUSE**:

```c
int (*getattr) (const char *, struct stat *);

//Get file attributes.

int (*readlink) (const char *, char *, size_t);

//Read the target of a symbolic link

int (*mknod) (const char *, mode_t, dev_t);

//Create a file node.

int (*mkdir) (const char *, mode_t);

//Create a directory.

int (*unlink) (const char *);

//Remove a file

int (*rmdir) (const char *);

//Remove a directory

int (*rename) (const char *, const char *);

//Rename a file

int (*chmod) (const char *, mode_t);

//Change the permission bits of a file

int (*chown) (const char *, uid_t, gid_t);

//Change the owner and group of a file

int (*truncate) (const char *, off_t);

//Change the size of a file

int (*open) (const char *, struct fuse_file_info *);

//File open operation.

int (*readdir) (const char *, void *, fuse_fill_dir_t, off_t, struct fuse_file_info *);

//Read directory

int (*read) (const char *, char *, size_t, off_t, struct fuse_file_info *);

//Read data from an open file

int (*write) (const char *, const char *, size_t, off_t, struct fuse_file_info *);

//Write data to an open file
```

### Making a FUSE Program

Fuse has a `struct` named `fuse_operations` which is defined as follows:

```c
static  struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .access = xmp_access,
    .readlink = xmp_readlink,
    .readdir = xmp_readdir,
    .mknod = xmp_mknod,
    .mkdir = xmp_mkdir,
    .symlink = xmp_symlink,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename,
    .link = xmp_link,
    .chmod = xmp_chmod,
    .chown = xmp_chown,
    .truncate = xmp_truncate,
    .utimens = xmp_utimens,
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write,
    .statfs = xmp_statfs,
    .create = xmp_create,
    .release = xmp_release,
    .fsync = xmp_fsync,
    .setxattr = xmp_setxattr,
    .getxattr = xmp_getxattr,
    .listxattr = xmp_listxattr,
    .removexattr = xmp_removexattr,
};
```

Every attribute on the `struct` is a _pointer_ that points to a function. Every function is called by FUSE when an event occurred on the _file system_. For example when _user_ write on a file, a function that is referenced by the attribute "write" on the struct `struct` will be called.

Moreover, the attribute on the `struct` is written like the command that is normally used on linux. For example when we make _directory_ in FUSE then mkdir function is called.

**To implement FUSE**, we need to use this `struct` and define the function that is refrenced in the `struct` tersebut. After, we reference every `struct` attribute to a pointer of a function which we have implement.

Most functions that are available is **optional**, we dont need to implement every function. Some functions are essentials to implement on the _file system_. Those functions are:

- `getattr` function is called whe the sistem needs to get attribute of a file.

- `readdir` function is called when the user try to list all files and directory inside a spesific directory.

- `read` function is called when the system try to read a file part by part.

To see all available function on **FUSE**, you can open the following link: [https://libfuse.github.io/doxygen/structfuse\_\_operations.html](https://libfuse.github.io/doxygen/structfuse__operations.html)

Here is an example program, by only implementing those 3 functions.

```c
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>


static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    res = lstat(path, stbuf);

    if (res == -1) return -errno;
    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;


    dp = opendir(path);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if(filler(buf, de->d_name, &st, 0)) break;
    }
    closedir(dp);
    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;
    (void) fi;


    fd = open(path, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
};

int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
```

We can _compile_ the code by using the following command

```
gcc -Wall `pkg-config fuse --cflags` [fuse.c] -o [output] `pkg-config fuse --libs`
```

We then make the directory for the destination of our FUSE file system and execute FUSE on that directory.

```
$ mkdir [destination directory]
$ ./[output] [destination directory]
```

After you execute the program, enter the destination directory. The content of that directory is similar to what is listed on the `root` or `/` directory.

### Unmount FUSE

Unmount command is used to "unmount" a file system that has been mounted, also inform any delayed read or write operation so that the file system can be detached safely.

To **unmount** FUSE, run the following command:

```
sudo umount [destination directory]
or
fusermount -u [destination directory]
```

### Tips

1. Debugging Fuse

One way of how to debug when programming fuse is using `printf` dan running the program by using the following command : `./[output] -f [destination directory]`. Where `-f` will keep the program running in the foreground so you can see what the `printf` output.

2. Modifying parameters value

In accordance to the explanation in the beggining where FUSE can modify the _file system_ in _userspace_ without modifying anything on the kernel code base, here we can modify the FUSE code such that the content of folder that we mount is `/home/[user]/Documents`.

Change your FUSE code to the following code:
**NOTE : change [user] to your user account**

```c
#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>

static  const  char *dirpath = "/home/[user]/Documents";

static  int  xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];


    sprintf(fpath,"%s%s",dirpath,path);

    res = lstat(fpath, stbuf);

    if (res == -1) return -errno;

    return 0;
}



static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if(strcmp(path,"/") == 0)
    {
        path=dirpath;
        sprintf(fpath,"%s",path);
    } else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;
    DIR *dp;
    struct dirent *de;
    (void) offset;
    (void) fi;

    dp = opendir(fpath);

    if (dp == NULL) return -errno;

    while ((de = readdir(dp)) != NULL) {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;
        res = (filler(buf, de->d_name, &st, 0));

        if(res!=0) break;
    }

    closedir(dp);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if(strcmp(path,"/") == 0)
    {
        path=dirpath;


        sprintf(fpath,"%s",path);
    }
    else sprintf(fpath, "%s%s",dirpath,path);

    int res = 0;
    int fd = 0 ;

    (void) fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1) return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1) res = -errno;

    close(fd);

    return res;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .readdir = xmp_readdir,
    .read = xmp_read,
};

int  main(int  argc, char *argv[])
{
    umask(0);
    return fuse_main(argc, argv, &xmp_oper, NULL);
}
```
This can happen because `const char *path` is a pointer variable that points to the path value that the user is currently using, so by changing the value of path the position of the user will move accordingly

3. WARNING

FUSE is fundamentally different from Docker container, files inside of fuse mount directory is the same files in the host filesystem, so if we edit/delete/add file/folder inside of fuse it will also affect the files on the host.

## Operating System

### What is Operating System?

An operating system is a single device that controls computer devices and their resources. The components of an operating system can be illustrated in the following diagram.

![os-diagram](./assets/os-diagram.png)

- **Hardware**: Represents the hardware used by the operating system to run programs.
- **Kernel**: The core of the operating system that manages resources and runs programs.
- **Shell**: The interface used by the user to interact with the operating system.
- **Application**: A program that is run by the operating system through the shell connected to the kernel.

### What is Kernel?

The kernel is the part of the operating system that is not directly visible, either in the computer hardware or software. The kernel handles events coming from hardware (interrupts) and from software (system calls), and manages access to computer resources.

An example of event management from hardware (interrupt handlers) is a keystroke on the keyboard which will then be translated into the corresponding character symbol in the input buffer. The program will then be able to retrieve the character from the input buffer.

While system calls will be accessed by the program, such as opening files, running other programs, and so on. Each system call handler will check the validity of the given arguments and then continue the given command.

Programs in general will not directly do system calls directly, but instead use a standard library to call them. The standard library will translate the given function into a form of system calls that matches what the kernel needs. An example is the `fopen()` function in the C language that will make system calls to open files.

The kernel is responsible for abstracting related commands such as files, processes, sockets, directories, memory, and others.

### What is Shell?

The shell is a program that is usually integrated with the distribution of the operating system. It serves as an interface between the user and the operating system. The form of the shell can differ from one operating system to another. It can be either a command line interface (CLI) or a graphical user interface (GUI).

The main concept of the shell is as follows.

- Users can open a program to run and can also provide arguments to the program.
- Allows users to perform operations on local storage, such as performing copy, move, delete, and others.

To perform these tasks, the shell will call several system calls which will be continued to the kernel.

### Operating System Booting Process

The operating system booting process is the process by which the operating system is run from a dead state until it is ready to be used by the user. In simple terms, here is the process of booting an operating system.

![boot-1](./assets/boot-1.png)

#### Basic Input Output System (BIOS)

BIOS consists of several instructions written in a very low level programming language. BIOS is the first program installed on a computer, even before the operating system. When the computer is turned on, the CPU will instruct the BIOS to check the basic hardware of the computer whether it is ready for use or not. This process is called **Power of Self Test** (POST). Some of the components that are checked during POST are as follows.

- Hardware, such as the processor, storage, and memory.
- Input/output devices, such as keyboard, mouse, and monitor.
- Registers of the CPU.
- Interrupt Controller

After POST is completed, it will be followed by the operating system boot process. The process of booting from storage by loading the first sector into memory is called the **Master Boot Record** (MBR).

#### Master Boot Record (MBR)

The MBR is the first sector of the disk/storage system. The physical location of the MBR is on _cylinder_ 0, _head_ 0, and _sector_ 1. The MBR stores partition table information and also the bootloader. Bootloader is a program that is responsible for loading the operating system into memory. The size of the MBR is 512 bytes. The first 466 bytes contain the bootloader, the next 64 bytes contain the partition table which consists of 4 partitions of 16 bytes each. Then on the last 2 bytes, the MBR contains the magic number (`0xAA55`). This _Magic number_ is used to signify that the MBR is valid.

![boot-2](./assets/boot-2.png)

The job of the MBR is to look for the second bootloader in the partition table. After finding the second bootloader, the MBR will load the second bootloader into memory and execute it.

#### Kernel Loader

The second bootloader loaded by the MBR is the kernel loader. The kernel loader is responsible for loading the kernel of the operating system into memory. The kernel loader will load the kernel into memory and will run the kernel.

In Linux, the kernel loader used is GRUB (Grand Unified Bootloader). GRUB is a bootloader that is open source and can be used on various operating systems. GRUB has several useful features, such as _multiboot_, _multiboot2_, and _chainloading_.

This is done because in modern kernel loaders such as GRUB, it is not enough to fit into 466 bytes in the MBR. So the MBR needs to find a second bootloader that will load the kernel into memory.

Here is an illustration of the Linux operating system booting process.

![boot-3](https://miro.medium.com/v2/resize:fit:720/format:webp/0*qdntCfi8fNxT-x0S.png)

### Interrupts

Interrupts are mechanisms used by the CPU to notify that an event has occurred. Interrupts are used to stop the execution of the running program and run the interrupt handler corresponding to the event.

An interrupt handler or interrupt service routine (ISR) is analogous to an event listener in event-driven programming, which is commonly used in high-level programming languages such as JavaScript. The ISR is an event handler for interrupts that occur. Each interrupt has an associated code that indicates the event that occurred. This code is called the interrupt vector. The instruction for interrupt on x86 is `int n` where `n` is the vector of interrupts. The interrupt vector can be written in hexadecimal notation (0x00 - 0xFF) or vector notation (00h - FFh).

More information about interrupts can be found in [here](https://en.wikipedia.org/wiki/BIOS_interrupt_call).

The implementation of interrupts that will be used in the practicum is service `int 21h` which is used to perform a software interrupt in the DOS kernel. By calling `int 21h` using certain parameters (such as AH and AL), we can perform various input output accesses on the DOS operating system.

## Simple OS Development

### Tools Installation

To create a simple operating system, we need some tools that will be used. Here are the necessary tools.

- **Bochs**: Bochs is an x86 emulator used to run the created operating system. Bochs can be run on various operating systems, such as Windows, MacOS, and Linux.

  Bochs can be downloaded on the [official bochs website](https://bochs.sourceforge.io/) or on the [following](https://github.com/bochs-emu/Bochs/releases/tag/REL_2_8_FINAL) github release. Adjust to the operating system you are using. **It is highly recommended to download the latest version of bochs through the website, not using apt.**

- **NASM**: NASM is an assembler used to convert assembly code into binary code. NASM can be downloaded at [official NASM website](https://www.nasm.us/) or can use the following command.

  ```bash
  sudo apt install nasm
  ```

- **BCC**: BCC is a C compiler used to compile C code to 16 bits (GCC cannot be used because GCC generates 32 bit code). The BCC documentation can be accessed [here](https://linux.die.net/man/1/bcc) and can be downloaded using the following command.

  ```bash
  sudo apt install bcc
  ```

- **ld86**: ld86 is a linker used to combine the object code of assembly code and C code into an executable.

  ```bash
  sudo apt install bin86
  ```

- **make**: Make is a build automation tool used to simplify the code compilation process. Make can be downloaded using the following command.

  ```bash
  sudo apt install make
  ```

### Preparing Disk Image

The operating system will be stored on a disk image. A disk image is a file that contains data that will be used by the operating system. The disk image will be used by the emulator to store the operating system that will be run. This time, a disk image with a size of 1.44 MB (floppy disk size) will be used. The disk image can be created using the following command.

```bash
dd if=/dev/zero of=floppy.img bs=512 count=2880
```

- `if=/dev/zero` `(input file = /dev/zero)`: Read from `/dev/zero` which contains byte 0.
- `of=floppy.img` `(output file = floppy.img)`: Writes to `floppy.img`.
- `bs=512` `(block size = 512)`: Sets the block size to 512 bytes.
- `count=2880`: Sets the number of blocks to 2880 (floppy disk size).

### Make Simple Bootloader

Before getting into creating a bootloader that calls the kernel, we will create a simple bootloader that only displays a black screen. Here is the simple bootloader code.

```asm
; simple-bootloader.asm
bits 16

times 510 - ($-$$) db 0x00
dw 0xAA55
```

- `bits 16`: Indicates that this code is a 16 bit code.

- `times 510 - ($-$$) db 0x00`: Fills the empty part of the MBR with 0x00.

  510 bytes minus the code size `($-$$)` is filled with 0x00

- `dw 0xAA55`: Indicates that the generated MBR is valid.

  The last 2 bytes are filled with 0xAA55

After that, the above code can be compiled using the following command.

```bash
nasm -f bin simple-bootloader.asm -o simple-bootloader.bin
```

- `-f bin` `(binary format)`: Converts assembly code into binary code.

Next, the resulting binary code will be merged with the previously created disk image using the following command.

```bash
dd if=simple-bootloader.bin of=floppy.img bs=512 count=1 conv=notrunc
```

- `count=1`: Write 1 block (according to MBR size 512 bytes).
- `conv=notrunc` `(convert = notruncate)`: Write to `floppy.img` without deleting existing data.

### Running Simple Bootloader

After creating a simple bootloader, we will run the bootloader using the Bochs emulator. The configuration can be used in the playground [`bochsrc.txt`](./playground/bochsrc.txt). It can then be run using the following command.

```bash
bochs -f bochsrc.txt
```

or if you are in the playground directory, you can use the following command.

```bash
bochs
```

After running it will appear as follows.

![simple-bootloader](./assets/simple-bootloader.png)

### Make Assembly Kernel

After creating the bootloader, we will create a kernel that will be called by the bootloader. Here is the kernel code.

```asm
; kernel.asm
global _putInMemory

; void putInMemory(int segment, int address, char character)
_putInMemory:
	push bp
	mov bp,sp
	push ds
	mov ax,[bp+4]
	mov si,[bp+6]
	mov cl,[bp+8]
	mov ds,ax
	mov [si],cl
	pop ds
	pop bp
	ret
```

- `global _putInMemory`: Declares the `_putInMemory` function as a global function that can later be called through C code.

- `_putInMemory`: Implementation of the `_putInMemory` function.

  The point of this function is to input characters into memory. This function will accept 3 arguments, namely segment, address, and character. Segment and address are used to determine the memory address that will be filled with the given character.

  This function was created because the memory address to be filled can exceed 16 bits. While the registers on x86 only have 16 bits. So we need segment and address to determine the memory address to be filled.

After that, the above code can be compiled using the following command.

```bash
nasm -f as86 kernel.asm -o kernel-asm.o
```

- `-f as86` `(format as86)`: Converts assembly code into object code.

### Make C Kernel

After creating the assembly kernel, we will create a C kernel that will call the `_putInMemory` function created earlier. Here is the C kernel code.

```c
// kernel.c
void main() {
  char* str = "Hello";
  int i = 0;

  for (i = 0; i < 4; i++) {
    char color = 0x5;
    putInMemory(0xB000, 0x8000 + i * 2, str[i]);
    putInMemory(0xB000, 0x8001 + i * 2, color);
  }

  while (1);
}
```

- `void main()`: The main function of the kernel.

- `0xB8000` is the memory address for the video memory of the VGA. This address is used to display characters at the top left of the screen at position `(0, 0)`.

- Since `0xB8000` cannot fit in 16 bits (2 bytes), we split the address into 2 parts, segment and address. Segment is `0xB000` and address is `0x8000`. The operation that the `_putInMemory` function will perform on the assembly is to combine the two addresses into a valid memory address.

- Offset for the character is `0x8000 + i * 2` and for the color is `0x8001 + i * 2`. This offset is used to display the character and color on the screen.

- Usually, the offset for the character on `(x,y)` is `0x8000 + (y * 80 + x) * 2`.

- While the offset for the color on `(x,y)` is `0x8001 + (y * 80 + x) * 2`.

- `while (1)`: _Infinite loop_ so that the program does not stop after displaying the character.

After that, the above code can be compiled using the following command.

```bash
bcc -ansi -c kernel.c -o kernel.o
```

- `-ansi`: Uses the ANSI C standard.
- `-c`: Generates an object file.
- `-o kernel.o`: Saves the compilation result into the `kernel.o` file.

### Linking Assembly and C Kernel

After creating the assembly kernel and C kernel, we will combine the two _object files_ into an executable. Here is the command used.

```bash
ld86 -o kernel.bin -d kernel.o kernel-asm.o
```

- `-o kernel`: Saves the result of linking into the `kernel` file.

- `-d`: Removes the header from the output file which result in the order of function declarations need to be sequential.

- `kernel.o kernel-asm.o`: Object file that will be merged.

### Extending Bootloader

After creating the kernel, we will change the bootloader that was created earlier so that it can call the kernel that has been created. Here is the modified bootloader code.

```asm
; bootloader.asm
bits 16

KERNEL_SEGMENT equ 0x1000 ; kernel will be loaded at 0x1000:0x0000
KERNEL_SECTORS equ 15     ; kernel will be loaded in 15 sectors maximum
KERNEL_START   equ 1      ; kernel will be loaded in sector 1

; bootloader code
bootloader:
  ; load kernel to memory
  mov ax, KERNEL_SEGMENT    ; load address of kernel
  mov es, ax                ; buffer address are in ES:BX
  mov bx, 0x0000            ; set buffer address to KERNEL_SEGMENT:0x0000

  mov ah, 0x02              ; read disk sectors
  mov al, KERNEL_SECTORS    ; number of sectors to read

  mov ch, 0x00              ; cylinder number
  mov cl, KERNEL_START + 1  ; sector number
  mov dh, 0x00              ; head number
  mov dl, 0x00              ; read from drive A

  int 0x13                  ; call BIOS interrupts

  ; set up segment registers
  mov ax, KERNEL_SEGMENT
  mov ds, ax
  mov es, ax
  mov ss, ax

  ; set up stack pointer
  mov ax, 0xFFF0
  mov sp, ax
  mov bp, ax

  ; jump to kernel
  jmp KERNEL_SEGMENT:0x0000

  ; padding to make bootloader 512 bytes
  times 510-($-$$) db 0
  dw 0xAA55
```

The assembly code above will load the kernel into memory and will run the kernel. The kernel will run at memory address `0x1000:0x0000`. After that, the above code can be compiled using the following command.

```bash
nasm -f bin bootloader.asm -o bootloader.bin
```

### Putting Bootloader and Kernel into Disk Image

After creating the bootloader and kernel, we will combine the two binaries into a disk image. Here are the commands to use.

```bash
dd if=/dev/zero of=floppy.img bs=512 count=2880
dd if=bootloader.bin of=floppy.img bs=512 count=1 conv=notrunc
dd if=kernel.bin of=floppy.img bs=512 seek=1 count=15 conv=notrunc
```

- `seek=1`: Skip 1 block (according to MBR size 512 bytes).

### Running Simple OS

After creating the disk image containing the bootloader and kernel, we will run the simple operating system that has been created using the Bochs emulator. The configuration in the playground [`bochsrc.txt`](./playground/bochsrc.txt) can be used. It can then be run using the following command.

```bash
bochs -f bochsrc.txt
```

or if you are in the playground directory, you can use the following command.

```bash
bochs
```

After running, the following display will appear.

![simple-os](./assets/simple-os.png)

It can be seen that there is a `Hello` on the screen. The writing comes from the kernel code that was created earlier.

### Automate Build Process

To simplify the compilation process, we can use a build automation tool such as `make`. Here is an example of a `Makefile` that can be used.

```makefile
prepare:
	dd if=/dev/zero of=floppy.img bs=512 count=2880

bootloader:
	nasm -f bin bootloader.asm -o bootloader.bin
	dd if=bootloader.bin of=floppy.img bs=512 count=1 conv=notrunc

kernel:
	nasm -f as86 kernel.asm -o kernel-asm.o
	bcc -ansi -c kernel.c -o kernel.o
	ld86 -o kernel.bin -d kernel.o kernel-asm.o
	dd if=kernel.bin of=floppy.img bs=512 seek=1 conv=notrunc

build: prepare kernel bootloader
```

So to perform the compilation process, we only need to run the following command.

```bash
make build
```

# Excercises

1. Make a file system that points to /home/[user]/Downloads, the file system is called JagoFS, such that every file name on the file system have Jago* as a prefix like Jago*[nama_file]. For example examplefile.txt is shown as Jago_examplefile.txt. The file system only have read access. Another example is Jago_exampletwo.txt is shown as Jago_Jago_exampletwo.txt.
2. Make a file system that leads to /home/[user]/Documents. This file system has a logging feature, where when any file on the folder is modified, the change event is recorded on a lof file with the following format <filename>.<time:date>.<extension>. If the log file doesn't exist then the system will automatically make the log file.

# References

1. https://www.cs.hmc.edu/~geoff/classes/hmc.cs135.201109/homework/fuse/fuse_doc.html
2. http://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/
3. https://github.com/asayler/CU-CS3753-PA5
4. http://amazingharry.blogspot.co.id/2015/01/filesystem-in-userspace-fuse-in-linux_5.html
5. [https://docs.oracle.com/database/121/ADLOB/adlob_fs.htm#ADLOB45989](https://docs.oracle.com/database/121/ADLOB/adlob_fs.htm#ADLOB45989)
6. [http://www.fieldses.org/~bfields/kernel/vfs.txt](http://www.fieldses.org/~bfields/kernel/vfs.txt)
7. [https://developer.ibm.com/technologies/linux/tutorials/l-virtual-filesystem-switch/](https://developer.ibm.com/technologies/linux/tutorials/l-virtual-filesystem-switch/)
8. [https://wiki.osdev.org/Introduction](https://wiki.osdev.org/Introduction)
9. [https://medium.com/@ibshafique/the-linux-boot-process-180fb07af452](https://medium.com/@ibshafique/the-linux-boot-process-180fb07af452)
10. [https://en.wikipedia.org/wiki/BIOS_interrupt_call](https://en.wikipedia.org/wiki/BIOS_interrupt_call)
11. [https://en.wikipedia.org/wiki/DOS_API](https://en.wikipedia.org/wiki/DOS_API)
12. [https://wiki.osdev.org/Text_mode](https://wiki.osdev.org/Text_mode)
