# Modul 4 - FUSE and Simple Kernel

## Learning Outcomes

1. Understand the file system on the operating system
2. Can create a simple FUSE program
3. Understand the steps of creating an operating system

## List of Content

- [i. Learning Outcomes](#learning-outcomes)
- [ii. Table of Contents](#table-of-contents)
- [1. File System](#file-system)
  - [1.1. Types of File System](#1-types-of-file-system)
  - [1.2. Virtual File System](#2-virtual-file-system)
  - [1.3. Dentry](#3-dentry)
  - [1.4. Superblock](#4-superblock)
  - [1.5. Inode](#5-inode)
- [2. File System in Userspace](#file-system-in-userspace)
  - [2.1. Definition of FUSE](#definition-of-fuse)
  - [2.2. FUSE Installation](#1-fuse-installation)
  - [2.3. How FUSE Works](#2-how-fuse-works)
  - [2.4. Making a FUSE Program](#3-making-a-fuse-program)
  - [2.5. Unmount FUSE](#4-unmount-fuse)
- [3. Creation of Operating System](#creation-of-operating-system)
  - [3.1. Definition of Operating System](#definition-of-operating-system)
  - [3.2. Definition of Bootloader](#definition-of-bootloader)
  - [3.3. Definition of Kernel](#definition-of-kernel)
  - [3.4. Definition of Interrupt 0x21](#definition-of-interrupt-0x21)

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

-  _Device_: Is the _device identifier_, for example **/ dev / hda1** is the first _harddisk_ on the system having device identifier **0 × 300**.

-  _Inode Pointer_: Is a _pointer_ which points to the first inode of the filesystem.

-  _Blocksize_: Indicates the size of a _block_ file system, for example **1024** **bytes**. 

-  _Superblock Operation_: Is a pointer to a set of superblock routines (functions) from the _file system_, for example **read**, **write**, and so on.

-  _File System Type_: Shows the type of _file system_, for example: EXT2, FAT, NTFS.

-  _File System Specific_: It is a pointer to the information required by the _file system.

  ![enter image description here](./assets/figure3.gif?raw=true)



### Inode


An inode is a VFS abstraction for files. Each file, directory, and other data on VFS is represented by one and only one VFS inode. VFS inodes exist only in kernel memory and are stored in the inode cache as long as they are needed by the system. The information stored by the VFS Inode includes:

-  _Device_: Shows the device identifier of a device that stores files or a _directory_.

-  _Inode Number_: Is a unique inode number in the _file system_.

- Mode: Describes what the VFS inode represents.

-  _User_ ID: Is an identifier for the owner of the file.

-  _Time_: Indicate when the creation, modification, and writing of a file.

-  _Blocksize_: Shows the size of the _block_ used by the file.

-  _Inode Operations_: Is _pointer_ to a _routine_ who perform various operations on inode.

-  _Count_: Shows the number of times a system has used an inode.

-  _Lock_: Used to lock the VFS inode.

-  _File System Specific Information_: Indicates specific information required by an inode.

![enter image description here](./assets/figure4.gif?raw=true)

Following is the relationship between dentry, superblock, and inode on a Virtual File System.

![enter image description here](./assets/daHCZ.gif?raw=true)

![enter image description here](./assets/dentry.JPG?raw=true)


## File System in Userspace


### Definition of FUSE

FUSE (Filesystem in Userspace) is an  interface where we could make our own file system inside a userspace on linux.

The advantage of using FUSE is that we can use any available library to create your own file system without having to deeply understand what the file system is actually doing in the kernel space. This is done because the FUSE module can bridge the file system code in the userspace with the file system in the kernel space. Some of the other benefits of FUSE are as follows:

- Can be loaded and installed by ordinary users, for network access, to get archived files, for removable media, etc.
- If the FUSE system driver crashes, it will not affect the kernel.
- FUSE can be deployed quickly, both because it does not require administrator intervention to install and because it can be easily accessed by any supported OS.
- No license issues related to static connection with the kernel.

![Fuse Illustration](./assets/fuse.png?raw=true)


One interesting example of FUSE is [GDFS][7bb7b7cc](Google Drive File System), where this GDFS allows us to mount our Google Drive to a linux system and use it like a regular linux file.

[7bb7b7cc]: https://github.com/robin-thomas/GDFS  "GDFS"

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

-  ```fuse_main()``` (lib/helper.c) = as the main (userspace) function, the user program calls the fuse_main() function then the fuse_mount() function is called.
-  ```fuse_mount()``` (lib/mount.c) = creates a UNIX domain socket, then forks and creates a child process that runs fusermount
-  ```fusermount()``` (util/fusermount.c) = to check whether the FUSE module has been loaded. Then open /dev/fuse and send the handling file via the UNIX domain socket back to the fuse_mount() function.
-  ```fuse_new()``` (lib/fuse.c) = creates a data structure containing the space used to store the file system data
-  ```fuse_loop()``` (lib/fuse.c) = read file system calls from /dev/fuse

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

Fuse has a ```struct``` named ```fuse_operations``` which is defined as follows:

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

Every attribute on the ```struct``` is a _pointer_ that points to a function. Every function is called by FUSE when an event occurred on the *file system*. For example when _user_ write on a file, a function that is referenced by the attribute "write" on the struct ```struct``` will be called.



Moreover, the attribute on the ```struct``` is written like the command that is normally used on linux. For example when we make _directory_ in FUSE then mkdir function is called.



**To implement FUSE**, we need to use this ```struct``` and define the function that is refrenced in the ```struct``` tersebut. After, we reference every ```struct``` attribute to a pointer of a function which we have implement.



Most functions that are available is **optional**, we dont need to implement every function. Some functions are essentials to implement on the _file system_. Those functions are:

- ```getattr``` function is called whe the sistem needs to get attribute of a file.

- ```readdir``` function is called when the user try to list all files and directory inside a spesific directory.

- ```read``` function  is called when the system try to read a file part by part.

To see all available function on **FUSE**, you can open the following link: [https://libfuse.github.io/doxygen/structfuse__operations.html](https://libfuse.github.io/doxygen/structfuse__operations.html)

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

After you execute the program, enter the destination directory. The content of that directory is similar to what is listed on the ```root``` or ```/``` directory.


### Tips
One way of how to debug when programming fuse is using  `printf` dan running the program by using the following command : `./[output] -f [destination directory]`. Where `-f` will keep the program running in the foreground so you can see what the `printf` output.


In accordance to the explanation in the beggining where FUSE can modify the  _file system_ in _userspace_ without modifying anything on the kernel code base, here we can modify the FUSE code such that we show the content of ```/home/[user]/Documents```.

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

### Unmount FUSE
Unmount command is used to "unmount" a file system that has been mounted, also inform any delayed read or write operation so that the file system can be detached safely.

To **unmount** FUSE, run the following command:
```
sudo umount [destination directory]
or
fusermount -u [destination directory]
```


## Creation of Operating System

### Definition of Operating System

### Definition of Bootloader

### Definition of Kernel

### Definition of Interrupt 0x21


# Excercises

1. Make a file system that points to /home/[user]/Downloads, the file system is called JagoFS, such that every file name on the file system have Jago_ as a prefix like Jago_[nama_file]. For example examplefile.txt is shown as Jago_examplefile.txt. The file system only have read access. Another example is Jago_exampletwo.txt is shown as Jago_Jago_exampletwo.txt.
2. Make a file system that leads to /home/[user]/Documents. This file system has a logging feature, where when any file on the folder is modified, the change event is recorded on a lof file with the following format <filename>.<time:date>.<extension>. If the log file doesn't exist then the system will automatically make the log file.

# References

1. https://www.cs.hmc.edu/~geoff/classes/hmc.cs135.201109/homework/fuse/fuse_doc.html
2. http://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/
3. https://github.com/asayler/CU-CS3753-PA5
4. http://amazingharry.blogspot.co.id/2015/01/filesystem-in-userspace-fuse-in-linux_5.html
5. [https://docs.oracle.com/database/121/ADLOB/adlob_fs.htm#ADLOB45989](https://docs.oracle.com/database/121/ADLOB/adlob_fs.htm#ADLOB45989)
6. [http://www.fieldses.org/~bfields/kernel/vfs.txt](http://www.fieldses.org/~bfields/kernel/vfs.txt)
7. [https://developer.ibm.com/technologies/linux/tutorials/l-virtual-filesystem-switch/](https://developer.ibm.com/technologies/linux/tutorials/l-virtual-filesystem-switch/)
