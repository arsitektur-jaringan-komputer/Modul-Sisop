
## 5. Making a Process

[Table of contents](#table-of-contents)

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


