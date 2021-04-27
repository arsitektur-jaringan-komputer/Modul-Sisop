### 1.3 Join Thread
Join thread is a function to join other threads that have stopped (*terminated*). If the thread that you want to join has not been stopped, this function will wait until the desired thread has a status of **`Terminated`**. The `pthread_join ()` function can be said to be the `wait ()` function of the process, because the main program (*task*) will wait for the thread to be joined in the main program. We do not know wether the main program or thread that completed the work first.

Example C program Join_Thread [thread_join.c](thread_join.c):

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //library thread

void *print_message_function( void *ptr );

int main()
{
    pthread_t thread1, thread2;//initial initialization
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int  iret1, iret2;

    iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1); //making the first thread
    if(iret1) //if error
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }

    iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);//making the second thread
    if(iret2)//if it fails
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret2);
        exit(EXIT_FAILURE);
    }

    printf("pthread_create() for thread 1 returns: %d\n",iret1);
    printf("pthread_create() for thread 2 returns: %d\n",iret2);

    // pthread_join( thread1, NULL);
    // pthread_join( thread2, NULL); 

    exit(EXIT_SUCCESS);
}

void *print_message_function( void *ptr )
{
    char *message;
    message = (char *) ptr;
    printf("%s \n", message);

    for(int i=0;i<10;i++){
        printf("%s %d \n", message, i);
    }
}

```

Explanation :
- In the above program, if we *comment* the line `pthread_join`, the result will not show the words **Thread 1** and **Thread 2**.
- If the `pthread_join` function call is uncommented, then the program we create will display the words **Thread 1** and **Thread 2**.

**Conclusion** :
The first program does not run the `print_message_function` function because before the second thread is scheduled, the main program (possibly) has finished executing so it doesn't run the default function on the thread. In the second experiment, the `pthread_join ()` function is used to make the main program wait for the thread to *join* until the target thread has finished executing, with this function the main program is suspended until the target thread has finished executing.
- Function for thread termination
  ```c
  #include <pthread.h>
  void pthread_exit(void *rval_ptr);
  ```
  The `rval_ptr` argument is a pointer that can be accessed by the `pthread_join ()` function in order to know the status of the thread

- Function to join thread
  ```c
  int pthread_join(pthread_t thread, void **rval_ptr);
  /* If successful returns value 0, if error returns value 1 */
  ```
 The function will pause the job until the `rval_ptr` pointer state of the` pthread_exit () `function returns its value.

### 1.4 Mutual Exclusion
Also known as ** Mutex **, which is a way to ensure that if a job that uses variables or files is also used by another job, the other job will output the value of the previous job.

Example of Simple Mutual_Exclusion program [threadmutex.c](threadmutex.c):
```c
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
 
pthread_t tid1, tid2;
int status;
int number;
 
void* write(void *arg)
{
    status = 0;
 
    printf("Input number : ");
    scanf("%d", &number);
 
    status = 1;
 
    return NULL;
}

void* read(void *arg)
{
    while(status != 1)
    {

    }

    printf("Number %d\n", number);
}
 
int main(void)
{

    pthread_create(&(tid1), NULL, write, NULL);
    pthread_create(&(tid2), NULL, read, NULL);
 
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
 
    return 0;
}

```

Explanation :
- There are 2 threads running with different functions.
- The resources (variables) that both threads use to execute their jobs **are the same**.
- The `status` variable is a simple example of controlling the running of a thread.

**Conclusion** :
Since we don't know which *thread* executes a variable or resource in the program first, the purpose of **Mutex** is to keep the resources of a thread from being used by other threads before it finishes its work.

## 2. IPC (Interprocess Communication)
### 2.1 IPC
IPC (*Interprocess Communication*) is a method to exchange data between one process and another, either on the same computer or remote computers connected through a network.

### 2.2 Pipes
Pipes are a sequential method of communication between interrelated processes. The weakness of pipes is that they are only for interconnected, sequential processes.
There are two types of pipes:
- unnamed pipe: Communication between parent and child processes.
- named pipes: Commonly referred to as FIFO, used for communication that runs independently. **Can only be used if both processes are using the same *filesystem***
```
$ ls | less
```
Diagram :

![alt](img/pipe.png "Diagram Pipe")  

Syntax in C languange :
```
int pipe(int fds[2]);

Parameters :
fd[0] will be the fd(file descriptor) for the 
read end of pipe.
fd[1] will be the fd for the write end of pipe.
Returns : 0 on Success.
-1 on error.
```
Example :  
[pipe1.c](pipe1.c)

```c
// C program to illustrate 
// pipe system call in C 
#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h>

#define MSGSIZE 16 
char* msg1 = "hello, world #1"; 
char* msg2 = "hello, world #2"; 
char* msg3 = "hello, world #3"; 

int main() 
{ 
	char inbuf[MSGSIZE]; 
	int p[2], i; 

	if (pipe(p) < 0) 
		exit(1); 

	/* continued */
	/* write pipe */

	write(p[1], msg1, MSGSIZE); 
	write(p[1], msg2, MSGSIZE); 
	write(p[1], msg3, MSGSIZE); 

	for (i = 0; i < 3; i++) { 
		/* read pipe */
		read(p[0], inbuf, MSGSIZE); 
		printf("%s\n", inbuf); 
	} 
	return 0; 
} 

```  
Output :  
```
hello, world #1
hello, world #2
hello, world #3
```  
Pipe with fork  
Diagram :  
![alt](img/pipe-fork.png)  

Example :  
[pipe-fork](pipe-fork.c)  
```c
// C program to demonstrate use of fork() and pipe() 
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 

int main() 
{ 
	// We use two pipes 
	// First pipe to send input string from parent 
	// Second pipe to send concatenated string from child 

	int fd1[2]; // Used to store two ends of first pipe 
	int fd2[2]; // Used to store two ends of second pipe 

	char fixed_str[] = "forgeeks.org"; 
	char input_str[100]; 
	pid_t p; 

	if (pipe(fd1)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 
	if (pipe(fd2)==-1) 
	{ 
		fprintf(stderr, "Pipe Failed" ); 
		return 1; 
	} 

	scanf("%s", input_str); 
	p = fork(); 

	if (p < 0) 
	{ 
		fprintf(stderr, "fork Failed" ); 
		return 1; 
	} 

	// Parent process 
	else if (p > 0) 
	{ 
		char concat_str[100]; 

		close(fd1[0]); // Close reading end of first pipe 

		// Write input string and close writing end of first 
		// pipe. 
		write(fd1[1], input_str, strlen(input_str)+1); 
		close(fd1[1]); 

		// Wait for child to send a string 
		wait(NULL); 

		close(fd2[1]); // Close writing end of second pipe 

		// Read string from child, print it and close 
		// reading end. 
		read(fd2[0], concat_str, 100); 
		printf("Concatenated string %s\n", concat_str); 
		close(fd2[0]); 
	} 

	// child process 
	else
	{ 
		close(fd1[1]); // Close writing end of first pipe 

		// Read a string using first pipe 
		char concat_str[100]; 
		read(fd1[0], concat_str, 100); 

		// Concatenate a fixed string with it 
		int k = strlen(concat_str); 
		int i; 
		for (i=0; i<strlen(fixed_str); i++) 
			concat_str[k++] = fixed_str[i]; 

		concat_str[k] = '\0'; // string ends with '\0' 

		// Close both reading ends 
		close(fd1[0]); 
		close(fd2[0]); 

		// Write concatenated string and close writing end 
		write(fd2[1], concat_str, strlen(concat_str)+1); 
		close(fd2[1]); 

		exit(0); 
	} 
} 

```

### 2.3 Sockets
*Socket* is an *end-point* in a process that communicates with each other. Usually *socket* is used for communication betwwen processes on different computers, but can also be used in the same computer 

Diagram :   
![alt](img/socket.png "implementasi socket C")

Example : [socket-server.c](socket-server.c) [socket-client.c](socket-client.c)

Server
```c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    valread = read( new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    return 0;
}
```
Client
```c
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 8080
  
int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Hello from client";
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    send(sock , hello , strlen(hello) , 0 );
    printf("Hello message sent\n");
    valread = read( sock , buffer, 1024);
    printf("%s\n",buffer );
    return 0;
}
```
Run the server process then run the client. Observe.

### 2.4 Message Queues
Is a communication between processes where the process creates an internal linked-list at the address of the Operating System kernel. The message is referred to as *queue* while the identifier is called *queue* ID. *Queue* ID is used as a *key* to indicate which message will be sent and the destination of the message.

### 2.5 Semaphores
Semaphores are different from other types of IPC. In its application, semaphore is a counter that is used for controlling resources by several processes simultaneously.
- If a memory block counter has a positive value, the semaphore can consume resources for the process, and reduce the counter block value by 1 to indicate that a memory block is being used.
- Conversely, if the semaphore is 0, the process will go into sleep mode until the semaphore is greater than 0.