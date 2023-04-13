# Module 2 Operating System

# Daemon and Process

In this module we will be using:
* Linux
* C Programming language (compile with _gcc_)

# List of Content

* Process and Daemon
    * [List of Content](#list-of-content)
    * [Process](#process)
        * [1. Definition](#1-definition)
        * [2. Types of PID](#2-types-of-pid)
            * [2.1 User ID (UID)](#21-user-id-uid)
            * [2.2 Process ID (PID)](#22-process-id-pid)
            * [2.3 Parent PID (PPID)](#23-parent-pid-ppid)
        * [3. See the Running Process](#3-see-the-running-process)
        * [4. Stop the Process](#4-stop-the-process)
        * [5. Making a Process](#5-making-a-process)
            * [fork](#fork)
            * [exec](#exec)
            * [Running Programs Concurrently](#running-programs-concurrently)
            * [wait x fork x exec](#wait-x-fork-x-exec)
            * [system](#system)
        * [6. Types of Processes](#6-types-of-processes)
            * [Zombie Process](#zombie-process)
            * [Orphan Process](#orphan-process)
            * [Daemon Process](#daemon-process)
    * [Daemon](#daemon)
        * [1. Definition of Daemon](#1-definition-of-daemon)
        * [2. Daemon Creation Steps](#2-daemon-creation-steps)
        * [3. Daemon Implementation](#3-daemon-implementation)
    * [Extras](#extras)
        * [Directory Listing in C](#directory-listing-in-c)
        * [File Permission in C](#file-permission-in-c)
        * [File Ownership in C](#file-ownership-in-c)
    * [Exercise](#exercise)

# Process

## 1. Definition

[List of Content](#list-of-content)

Process is a condition in which the OS runs (executes) a program. When a program is executed by the OS, the process has a PID (Process ID) which is the identifier of a process. On UNIX, to see the processes executed by the OS is by calling the shell command ```` ps ```` . To see more about the  ````ps```` command open ``` man ps```.

In its use, a process can form another process called **_spawning process_**. The process that calls another process is called **_parent process_** and the one that is called is **_child process_**.

## 2. Types of PID

[List of Content](#list-of-content)

### 2.1 User ID (UID)
Is an identifier of a process that displays the user running a program. In C programs, you can call the function ```uid_t getuid(void);```

### 2.2 Process ID (PID)
Unique number of an ongoing process to identify a process. In a C program, you can call the function ```pid_t getpid(void);```

### 2.3 Parent PID (PPID)
Each process has its own identifier and also after that process creates other processes. The process that is formed has an identifier in the form of the ID of the parent. In a C program, you can call the function ```pid_t getppid(void); ```.

## 3. See the Running Process

[List of Content](#list-of-content)

To see the processes currently running on the OS, you can use ```ps -ef``` to see the details.

![show ps](img/showps.png)

Explanation:
  * **UID**: the user who runs the program
  * **PID**: process ID
  * **PPID**: parent PID, if there is no parent, it will be 0
  * **C**: CPU Util. (%)
  * **STIME**: Time when the process started
  * **TTY**: Terminal associated with the process. If there is none, it means it's a background process
  * **TIME**: the length of the process running
  * **CMD**: command that executes the process

## 4. Stop the Process

[List of Content](#list-of-content)

To stop (_terminate_) a running process, run the shell command `` kill [options] <pid> ``. Usually to force stop a process you can use the command `` kill -9 <pid> ``. The number _9_ is the Signal code to terminate a process.

### Types of Signal

| Signal name | Signal value  | Effect       |
| ------------|:-------------:| -------------|
| SIGHUP      | 1             | Hangup         |
| SIGINT      | 2             | Interrupt from keyboard  |
| SIGKILL     | 9             | Kill signal   |
| SIGTERM     | 15            | Termination signal
| SIGSTOP     | 17,19,23      | Stop the process

By default when using the `` kill <pid> `` shell command, it will use ``SIGSTOP`` which will terminate the process but still can be resumed.

## 5. Making a Process

[List of Content](#list-of-content)

### **fork**
`fork` is a _system call_ function in C to perform _spawning process_. After calling the function, there will be a new process which is the _child process_, the function will return a value of 0 in the _child process_ but return the _PID_ of the newly spawned _child process_ in the _parent process_

Try the program below by compiling it with `gcc try.c -o try` and executing it with `./try`

```c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
  pid_t child_id;

  child_id = fork();

  printf("This will be called twice\n");

  if(child_id != 0){
    printf("\nParent process.\nPID: %d, Child's PID: %d\n", (int)getpid(), (int)child_id);
  }else {
    printf("\nChild process.\nPID: %d, Parent's PID: %d\n", (int)getpid(), (int)getppid());
  }

  return 0;
}
```

Result:
```c
This will be called twice

Parent process.
PID: 13101, Child's PID: 13102
This will be called twice

Child process.
PID: 13102, Parent's PID: 1
```


Visualization:

```c
+-------------------------+
|   Parent Process        |
+-------------------------+
|   int main() {          |
|     pid_t child_id;     |
|                         |
|     pid = getpid();     |
|     ppid = getppid();   |
|                         |
|-->  child_id = fork();  |
+-------------------------+
|    pid = 20             |
|    child_id = undefined |
|    ppid = 10            |
+-------------------------+
         |\
         | \----------------------------------\
         |                                     |
         V                                     V
+-------------------------+        +-------------------------+
|   Parent Process        |        |     Child Process       |
+-------------------------+        +-------------------------+
|-->                      |        |-->                      |
|     pid = getpid();     |        |     pid = getpid();     |
|     ppid = getppid();   |        |     ppid = getppid();   |
|   }                     |        |   }                     |
+-------------------------+        +-------------------------+
|    pid = 20             |        |    pid = 23             |
|    child_id = 23        |        |    child_id = 0         |
|    ppid = 10            |        |    ppid = 20            |
+-------------------------+        +-------------------------+
```

### **exec**

`exec` is a function to run a new program and replaces the currently running program. There are variations of the `exec` function such as `execvp`, `execlp`, dan `execv`.

The example below will use `execv`

```c
#include <stdio.h>
#include <unistd.h>

int main () {

  // argv[n] = { {your-program-name}, {argument[1]}, {argument[2]},.....,{argument[n-2]}, NULL }
  char *argv[4] = {"list", "-l", "/home/", NULL};

  execv("/bin/ls", argv);

  printf("This line will not be executed\n");

  return 0;

}
```

### **Running programs concurrently**

By combining ```fork``` and ```exec``, we can run 2 or more _tasks_ concurrently. An example is backing up different logs at the same time.


```c
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;

  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); // If it fails to create a new process, the program will stop 
  }

  if (child_id == 0) {
    // this is child

    char *argv[] = {"cp", "/var/log/apt/history.log", "/home/[user]/", NULL};
    execv("/bin/cp", argv);
  } else {
    // this is parent

    char *argv[] = {"cp", "/var/log/dpkg.log", "/home/[user]/", NULL};
    execv("/bin/cp", argv);
  }
}
```

Visualization:
```c
+--------+
| pid=7  |
| ppid=4 |
| bash   |
+--------+
    |
    | calls fork
    V                         
+--------+                     +--------+
| pid=7  |    forks            | pid=22 |
| ppid=4 | ------------------> | ppid=7 |
| bash   |                     | bash   |
+--------+                     +--------+
    |                              |
    | calls exec to run touch      | calls exec to run mkdir
    |                              |
    V                              V
```

If you want to do multiple tasks simultaneously disregarding the order, you can use `fork` and `exec`.


### **wait** x **fork** x **exec**

We can run two processes in one program. An example of its use is to create a folder and fill the folder with a file. First, create a folder. Then, create a file with the shell command `touch` in that folder. However, in reality, to do two things simultaneously requires a moment's pause.

To create files that are in a folder, the folder itself must first exist. To _delay_ a process can use the `wait` _system call_.


```c
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
  pid_t child_id;
  int status;

  child_id = fork();

  if (child_id < 0) {
    exit(EXIT_FAILURE); // If it fails to create a new process, the program will stop
  }

  if (child_id == 0) {
    // this is child

    char *argv[] = {"mkdir", "-p", "folderku", NULL};
    execv("/bin/mkdir", argv);
  } else {
    // this is parent
    while ((wait(&status)) > 0);
    char *argv[] = {"touch", "folderku/fileku.txt", NULL};
    execv("/usr/bin/touch", argv);
  }
}
```
In the example above, the `wait` function is to wait for _child process_ to finish doing its job, which is to create a folder. After _terminated_, the _parent process_ will resume running the process of creating `my files` in the` my folders` folder.

### **system**

`system` is a function to call shell commands directly from a C program. For example, when you want to call a script in a C program.` system (ls)` will produce the same output when calling it in a shell script with `ls`.

File examplebash.sh:
```sh
#!/bin/bash

echo "Shell script ran"
```

File system.c:
```c
#include <stdlib.h>

int main() {
  int return_value;
  return_value = system("bash examplebash.sh");
  return return_value;
}

```

Output:
```
Shell script ran
```


## 6. Types of Processes

[List of Content](#list-of-content)

### **Zombie Process**

The Zombie process occurs because there is a child process that is exited but the parent process does not know that the child process has ended, for example due to a network break. So that the parent process does not releases processes that are still used by the child processes even though the process is dead.

### **Orphan Process**

Orphan Process is a process on the computer where the parent process has finished or has stopped working, but the child process itself is still running.

### **Daemon Process**

Daemon Process is a process that runs in the background because it does not have a controlling terminal. In the Windows operating system it is usually better known as a service. Daemon is a process that is designed so that the process does not get intervention from a user.

---

# Daemon

[List of Content](#list-of-content)

## 1. Definition of Daemon
Daemon is a program that runs in the background continuously without direct interaction with an active user.

<!-- Sebuah daemon dapat berhenti karena beberapa hal. -->
## 2. Daemon Creation Steps
There are several steps to creating a daemon:
### 2.1 Fork the Parent Process and turn off the Parent Process
The first step is to create a parent process and generate the child process by `fork()`, then kill the parent process so that the operating system thinks the process is complete.

```c
pid_t pid;        // Variable to store pid

pid = fork();     // Store the pid of the child process

/* Exit when fork failed
 * (pid value < 0) */
if (pid < 0) {
  exit(EXIT_FAILURE);
}

/* Exit when fork succeed
 * (the value of pid is the child process's pid) */
if (pid > 0) {
  exit(EXIT_SUCCESS);
}
```

### 2.2 Changing File Mode with `umask`
Each file and directory has a _permissions_ that regulate who can read, write, and delete on the file or the directory.

Using `umask` we can set a  _permission_ from a file when it is created. Here we use  `umask(0)` so that we get full access of a file when a daemon created it.

```c
umask(0);
```

### 2.3 Creating Unique Session ID (SID)
A Child Process must have an SID in order to run. Without an SID, the Child Process whose Parent has been `kill`-ed will become an Orphan Process.

To get an SID we can use the command `setsid()`. This command has the same _return type_ as `fork()`.

```c
sid = setsid();
if (sid < 0) {
  exit(EXIT_FAILURE);
}
```

### 2.4 Changing Working Directory
A working directory must be changed to a directory that is sure to exist. To be safe, we will change it to the root (/) directory because it is a guaranteed directory on all Linux distributions.

To change the Working Directory, we can use the command `chdir()`.

```c
if ((chdir("/")) < 0) {
  exit(EXIT_FAILURE);
}
```

### 2.5 Closing the Standard File Descriptor
A daemon cannot use the terminal. Therefore we must close the standard file descriptors (STDIN, STDOUT, STDERR).

```c
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
```

### 2.6 Creating the Main Loop
This is the main loop where we write the gist of our program. Don't forget to give the command `sleep()` so that the loop runs at an interval.

```c
while (1) {
  // Write your program here

  sleep(30);
}
```


## 3. Daemon Implementation
The code below is the result of following the steps of making daemon process (Daemon Template)

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

int main() {
  pid_t pid, sid;        // Variables to store PID

  pid = fork();     // Storing PID of child process

  /* Exit the program if fork failed
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Exit the program if fork succeeded
  * (the value of pid is equal to the child process) */
  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();
  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while (1) {
    // Write your program here

    sleep(30);
  }
}
```
### 3.1 Compiling daemon program
To the daemon process, first we need to compile the C file that we have made with the command `gcc [program_name.c] -o [output_file_name]`.

### 3.2 Running daemon program
After completing the last step, the executable file will appear. It can be run by exectuting `./output_file_name`

### 3.3 Check if the daemon process is running
To check what processes are running, we can use the `ps -aux` command. To find the daemon process that we ran, we can use the `grep` command. We can combine both command to be `ps -aux | grep "output_file_name"`. If there is an output, that means the daemon process is running.

### 3.4 Terminating daemon process
To terminate daemon processes, we can use the `kill` command. First we need to fild the pid of the daemon process that we want to terminate. We can find the pid by using the last step. Then we can run `sudo kill -9 pid` to shut the process down. The "pid" is replaced by the pid of your daemon process.

# Extras

## Directory Listing in C
With C, we can check what are the contents of a directory. To achieve this, we need to use a spesific library called `dirent.h`.
Here are the example of directory listing in C:
```c
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>


int main (void)
{
    DIR *dp;
    struct dirent *ep;
    char path[100];

    printf("Enter path to list files: ");
    scanf("%s", path);

    dp = opendir(path);

    if (dp != NULL)
    {
      while ((ep = readdir (dp))) {
          puts (ep->d_name);
      }

      (void) closedir (dp);
    } else perror ("Couldn't open the directory");

    return 0;
}
```

We can also traverse recursively to a directory. For example :
```c
#include <stdio.h>
#include <string.h>
#include <dirent.h>

void listFilesRecursively(char *path);


int main()
{
    char path[100];

    printf("Enter path to list files: ");
    scanf("%s", path);

    listFilesRecursively(path);

    return 0;
}

void listFilesRecursively(char *basePath)
{
    char path[1000];
    struct dirent *dp;
    DIR *dir = opendir(basePath);

    if (!dir)
        return;

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            printf("%s\n", dp->d_name);

            // Construct new path from our base path
            strcpy(path, basePath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFilesRecursively(path);
        }
    }

    closedir(dir);
}
```

## File Permission in C
We can see permissions of a file or directory in C language with a library called `sys/stat.h`. Here are some example of file permission checking with C :
```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main()
{
    struct stat fs;
    char filename[100];
    int r;

    printf("Enter path to list files: ");
    scanf("%s", filename);

    r = stat(filename,&fs);
    if( r==-1 )
    {
        fprintf(stderr,"File error\n");
        exit(1);
    }

    printf("Obtaining permission mode for '%s':\n",filename);

    /* file permissions are kept in the st_mode member */
    /* The S_ISREG() macro tests for regular files */
    if( S_ISREG(fs.st_mode) )
        puts("Regular file");
    else
        puts("Not a regular file");

    printf("Owner permissions: ");
    if( fs.st_mode & S_IRUSR )
        printf("read ");
    if( fs.st_mode & S_IWUSR )
        printf("write ");
    if( fs.st_mode & S_IXUSR )
        printf("execute");
    putchar('\n');

    printf("Group permissions: ");
    if( fs.st_mode & S_IRGRP )
        printf("read ");
    if( fs.st_mode & S_IWGRP )
        printf("write ");
    if( fs.st_mode & S_IXGRP )
        printf("execute");
    putchar('\n');

    printf("Others permissions: ");
    if( fs.st_mode & S_IROTH )
        printf("read ");
    if( fs.st_mode & S_IWOTH )
        printf("write ");
    if( fs.st_mode & S_IXOTH )
        printf("execute");
    putchar('\n');
  
    return(0);
}
```
Variables that use the prefix `S_...` has some sort of a rule like file permission in linux. Here is an image showing how to use them :

![file-permission](img/file-permission.png)

## File Ownership in C
We can also see the owner and group of a file in C. This thing can be done with the library `sys/stat.h`, `pwd.h`, and `grp.h`. To get that information, 2 steps are needed, which is UID and GID of a file. Then we can find the name of the user and group from the user database or group database. Here is how to use it :
```c
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <sys/stat.h>

int main()
{
    struct stat info;
    char path[100];
    int r;

    printf("Enter path to list files: ");
    scanf("%s", path);

    r = stat(path, &info);
    if( r==-1 )
    {
        fprintf(stderr,"File error\n");
        exit(1);
    }

    struct passwd *pw = getpwuid(info.st_uid);
    struct group  *gr = getgrgid(info.st_gid);

    if (pw != 0) puts(pw->pw_name);
    if (gr != 0) puts(gr->gr_name);
}
```

# Exercise

[List Of Content](#list-of-content)
1. Modify the code [soal1.c](soal1.c) such that the output can be sorted from 0 to 100, without deleting existing function and using **wait**.
2. Make a program that copies a folder and all its content in */home/{USER}/Downloads* to a folder with this name format *tanggal-bulan-tahun_jam:menit:detik* (contoh: 26-03-2021_16:22:09). **Use fork, exec, dan wait**.
3. Make a daemon that runs every 10 seconds to backup the content of *error.txt* file that is saved in a file named *error.log.{no}* (example: error.log.1 , error.log.2, … ) then delete the content of *error.txt* such that the file is empty again. **You cannot use exec and system**

## References
* https://notes.shichao.io/apue/ch8/
* https://www.geeksforgeeks.org/exec-family-of-functions-in-c/
* http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
* https://www.computerhope.com/unix/uumask.htm
* http://www.gnu.org/savannah-checkouts/gnu/libc/manual/html_node/Simple-Directory-Lister.html
* https://codeforwin.org/2018/03/c-program-to-list-all-files-in-a-directory-recursively.html
* https://c-for-dummies.com/blog/?p=4101
* https://pubs.opengroup.org/onlinepubs/009695399/functions/getgrgid.html
* https://pubs.opengroup.org/onlinepubs/009695399/functions/getgrgid.html
* https://pubs.opengroup.org/onlinepubs/009695399/functions/getpwuid.html
