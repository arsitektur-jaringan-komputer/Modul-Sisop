## Modul 2 Praktikum Sistem Operasi

# Proses dan Daemon

Requirements:
1. Linux
2. gcc / g++

Tutorial compiling C code: [here](https://github.com/raldokusuma/compile-c-program)

## Daftar Isi
- [Proses dan Daemon](#proses-dan-daemon)
  - [Daftar Isi](#daftar-isi)
  - [1. Proses](#1-proses)
    - [1.1 Proses](#11-proses)
    - [1.2 Proses ID](#12-proses-id)
      - [Process ID (PID)](#process-id-pid)
      - [Parent Process ID (PPID)](#parent-process-id-ppid)
      - [Parent Process](#parent-process)
      - [Child Process](#child-process)
    - [1.3 Melihat Proses](#13-melihat-proses)
    - [1.4 Membunuh Proses](#14-membunuh-proses)
    - [1.5 Membuat Proses](#15-membuat-proses)
      - [fork](#fork)
      - [exec](#exec)
      - [fork and exec](#fork-and-exec)
      - [wait](#wait)
      - [system](#system)
    - [1.6 Jenis-Jenis Proses](#16-jenis-jenis-proses)
      - [Zombie Process](#zombie-process)
      - [Orphan Process](#orphan-process)
      - [Daemon Process](#daemon-process)
  - [2. Daemon](#2-daemon)
    - [2.1 Daemon](#21-daemon)
    - [2.2 Proses Pembuatan Daemon](#22-proses-pembuatan-daemon)
      - [1. Fork Parent Process dan penghentian Parent Process](#1-fork-parent-process-dan-penghentian-parent-process)
      - [2. Mengubah mode file menggunakan `umask(0)`](#2-mengubah-mode-file-menggunakan-umask0)
      - [3. Membuat Unique Session ID (SID)](#3-membuat-unique-session-id-sid)
      - [4. Mengubah Directory Kerja](#4-mengubah-directory-kerja)
      - [5. Menutup File Descriptor Standar](#5-menutup-file-descriptor-standar)
      - [6. Membuat Loop utama](#6-membuat-loop-utama)
    - [2.3 Contoh Implementasi Daemon](#23-contoh-implementasi-daemon)
  - [Appendix](#appendix)
    - [Soal Latihan](#soal-latihan)
      - [Latihan 1](#latihan-1)
      - [Latihan 2](#latihan-2)
      - [Latihan 3](#latihan-3)
    - [References](#references)


## 1. Proses

### 1.1 Proses
Proses merupakan keadaan ketika sebuah program sedang di eksekusi. Setiap proses juga memiliki PID atau Process ID yang merupakan nomor unik yang dapat digunakan untuk berinteraksi dengan proses bersangkutan.

Jalankan `$ ps` untuk menampilkan proses atau `$ ps -u` untuk lebih detailnya.

### 1.2 Proses ID
#### Process ID (PID)
PID merupakan indentitas unik berupa angka yang digunakan dalam beberapa sistem operasi untuk mengidentifikasi suatu proses. Untuk mendapatkan PID gunakan perintah `System call getpid()`
#### Parent Process ID (PPID)
PPID merupakan induk dari PID dan merupakan creator dari suatu proses. Setiap proses memiliki satu induk proses (PPID). Untuk mendapatkan PPID gunakan perintah `System call getppid()`
#### Parent Process
Parent process merupakan proses yang menciptakan beberapa child process. Proses ini tercipta dengan mengeksekusi fungsi `fork`, lalu hasil pemanggilan fungsi `fork` tersebut menciptakan beberapa child process.
#### Child Process
Child process merupakan proses yang dibuat oleh parent process. Setiap proses dapat membuat banyak child process, namun setiap proses hanya memiliki satu parent process, kecuali untuk proses yang paling pertama, proses tersebut tidak memiliki parent. Proses pertama yang dipanggil unit dalam Linux dimulai oleh kernel pada saat boot dan tidak pernah berhenti.

### 1.3 Melihat Proses
Melihat proses yang sedang berjalan menggunakan `$ ps -e`
```
  PID  PPID COMMAND
    1     0 /lib/systemd/systemd --system --deserialize 37
    2     0 [kthreadd]
    3     2 [kworker/0:0]
    4     2 [kworker/0:0H]
    6     2 [mm_percpu_wq]
    7     2 [ksoftirqd/0]
    8     2 [rcu_sched]
...........
(long list)
...........
23713  1844 /usr/lib/gnome-terminal/gnome-terminal-server
23767 23713 bash
23806 18599 /usr/lib/firefox/firefox -contentproc -childID 8 -isForBrowser -pref
23914     2 [kworker/u16:0]
23923     2 [kworker/u16:4]
23963 23767 ps -e -o pid,ppid,command
```

### 1.4 Membunuh Proses
Membunuh proses menggunakan `$ kill {pid}`

Contoh :
```
$ kill 3789
```
terdapat beberapa macam signal yang digunakan dalam command kill, antara lain sebagi berikut :

| Signal name | Signal value  | Effect       |
| ------------|:-------------:| -------------|
| SIGHUP      | 1             | Hangup         |
| SIGINT      | 2             | Interrupt from keyboard  |
| SIGKILL     | 9             | Kill signal   |
| SIGTERM     | 15            | Termination signal
| SIGSTOP     | 17,19,23      | Stop the process

Secara default, `$ kill` menggunakan signal SIGTERM. Untuk menggunakan signal tertentu, gunakan 
```
$ kill -{signal value} {pid}
```
. Contoh, `$ kill -9 3789` untuk menggunakan SIGKILL.

### 1.5 Membuat Proses
Untuk membuat proses ada dua cara umum yang sering digunakan, `fork` dan `exec`.
#### fork
`fork` digunakan untuk menduplikasi proses. Proses yang baru disebut dengan _child proses_, sedangkan proses yang memanggilnya disebut dengan _parent proses_. Setelah `fork` dipanggil, kita tidaklah tahu proses manakah yang pertama selesai.
Manual: `$ man 2 fork`

Tulislah kode berikut, simpan dalam format c.
```C
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;

  printf("This is the main program, with PID = %d, Child's ID = %d, Parent ID = %d\n", 
      (int) getpid(), (int) child_id, (int) getppid());

  child_id = fork();
  if (child_id != 0) {
    printf("This is the parent process, with PID = %d, Child's ID = %d, Parent ID = %d\n", 
      (int) getpid(), (int) child_id, (int) getppid());
  } else {
    printf("This is the child process, with PID = %d, Child's ID = %d, Parent ID = %d\n", 
      (int) getpid(), (int) child_id, (int) getppid());
  }

  return 0;
}
```
Coba jalankan dan perhatikan hasilnya. Semua perintah `printf` jalan, bagaimana bisa? Perhatikan PID, Child's ID, dan Parent ID setiap kalimat output.
```C
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
Perhatikan, bahwa;
- `pid` Parent Process        == `ppid` Child 
- `child_id` Parent Process   == `pid` Child Process

#### exec
`exec` adalah function yang digunakan untuk menjalankan program baru dan mengganti program yang sedang berlangsung. `exec` adalah program family yang memiliki berbagai fungsi variasi, yaitu `execvp`, `execlp`, `execv`, dan lain lain.

Manual: `$ man 3 exec`

Contoh:
```C
#include <stdio.h>
#include <unistd.h>

int main () {
  
  // argv[n] = { {your-program-name}, {argument[1]}, {argument[2]},.....,{argument[n-2]}, NULL }
  char *argv[4] = {"list", "-l", "/home/[user]/", NULL};
  
  execv("/bin/ls", argv);

  printf("This line will not be executed\n");

  return 0;
}
```

#### fork and exec
__Permasalahan:__
Bagaimana cara menjalankan dua proses dalam satu program?

__Contoh Permasalahan:__
Bagaimana cara membuat folder `~/sisop` dan membuat file kosong bernama `~/process.c`?

Maka, bagaimana cara menjalankan `mkdir` __dan__ `touch` dalam satu program?

__Solusi:__
Gunakan `fork` dan `exec`!

Buat sebuah program dengan:  
1. Buat proses baru dengan `fork`
2. Jalankan `exec` yang memanggil `mkdir` pada child process
3. Jalankan `exec` yang memanggil `touch` pada parent process

Visualisasi
```
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

Contoh:
```C
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;

  child_id = fork();
  
  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    // this is child
    
    char *argv[] = {"mkdir", "sample-dir", NULL};
    execv("/bin/mkdir", argv);
  } else {
    // this is parent
  
    char *argv[] = {"touch", "sample-touch.txt", NULL};
    execv("/usr/bin/touch", argv);
  }
}
```

#### wait
`wait` adalah function yang digunakan untuk mendapatkan informasi ketika child proses berganti _state_-nya. Pergantian state dapat berupa _termination_, _resume_, atau _stop_. Pada modul ini, kita hanya menggunakan `wait` untuk menangkap state _termination_.

Fungsi `wait` pada parent process juga berguna untuk menangkap exit status dari child.

__Permasalahan:__  
Bagaimana cara membuat program yang menjalankan suatu proses tanpa menghentikan program?

__Contoh permasalahan:__  
Bagaimana cara membuat folder `~/sisop` dan membuat file kosong bernama `~/process.c` __di dalamnya__?

Maka, bagaimana cara menjalankan `mkdir` __lalu__ menjalankan `touch` dalam satu program?

__Solusi:__  
Gunakan `fork`, `exec`, dan `wait`!

Buat sebuah program dengan:  
1. Buat proses baru dengan `fork`
2. Jalankan `exec` yang memanggil `mkdir` pada child process
3. Buat parent process menunggu (`wait`) hingga proses pada child selesai
4. Setelah child selesai, jalankan `exec` yang memanggil `touch` pada parent

Visualisasi
```
+--------+
| pid=7  |
| ppid=4 |
| bash   |
+--------+
    |
    | calls fork
    V
+--------+             +--------+
| pid=7  |    forks    | pid=22 |
| ppid=4 | ----------> | ppid=7 |
| bash   |             | bash   |
+--------+             +--------+
    |                      |
    | waits for pid 22     | calls exec to run mkdir
    |                      V
    |                  +--------+
    |                  | pid=22 |
    |                  | ppid=7 |
    |                  | ls     |
    V                  +--------+
+--------+                 |
| pid=7  |                 | exits
| ppid=4 | <---------------+
| bash   |
+--------+
    |
    | calls exec to run touch
    |
    V
```
Contoh:
```C
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;
  int status;

  child_id = fork();

  if (child_id == 0) {
    // this is child

    char *argv[4] = {"mkdir", "-p", "sample-dir", NULL};
    execv("/bin/mkdir", argv);
  } else {
    // this is parent

    // the parent waits for all the child processes
    while ((wait(&status)) > 0);

    char *argv[3] = {"touch", "sample-dir/sample-touch.txt", NULL};
    execv("/usr/bin/touch", argv);
  }
}
```
#### system
Ketika [system](http://man7.org/linux/man-pages/man3/system.3.html) dijalankan, program hanya memanggil _external command_ (kalau di Ubuntu berupa program `/bin/bash`). Penggunaan `system` sangat tergantung pada environment. 

Contoh, ketika user menjalankan `system("ls -l")`, ini sama seperti menjalankan `$ ls -l` pada bash.

Meskipun mudah digunakan, tidak disarankan menggunakan fungsi `system` karena beberapa asalan:
```
$ man system

  ....
  NOTES
  system() provides simplicity and convenience: it handles  all
  of  the details of calling fork(2), execl(3), and waitpid(2),
  as well as the necessary manipulations of signals.

  Do not use system() from a program with set-user-ID  or  set-
  group-ID privileges, because strange values for some environ‐
  ment variables might be used  to  subvert  system  integrity.
  ...
```

Contoh:
```C
#include <stdlib.h>

int main() {
  int return_value;
  return_value = system("ls -l /");
  return return_value;
}
```

Hasil
```
total 156
drwxr-xr-x   2 root root  4096 Sep 14 06:35 bin
drwxr-xr-x   4 root root  4096 Sep 20 00:24 boot
drwxrwxr-x   2 root root  4096 Agu 14 14:05 cdrom
drwxr-xr-x   3 root root  4096 Sep 12 19:11 data
(long list)
```

### 1.6 Jenis-Jenis Proses
#### Zombie Process
Zombie Process terjadi karena adaanya child process yang di exit namun parrent processnya tidak tahu bahwa child process tersebut telah di terminate, misalnya disebabkan karena putusnya network. Sehingga parent process tidak merelease process yang masih digunakan oleh child process tersebut walaupun process tersebut sudah mati.
#### Orphan Process
Orphan Process adalah sebuah proses yang ada dalam komputer dimana parent process telah selesai atau berhenti bekerja namun proses anak sendiri tetap berjalan.
#### Daemon Process
Daemon Process adalah sebuah proses yang bekerja pada background karena proses ini tidak memiliki terminal pengontrol. Dalam sistem operasi Windows biasanya lebih dikenal dengan sebutan service. Daemon adalah sebuah proses yang didesain supaya proses tersebut tidak mendapatkan intervensi dari user.

## 2. Daemon
### 2.1 Daemon
Daemon adalah proses yang berjalan di balik layar (background) dan tidak berinteraksi langsung dengan user melalui standard input/output.
### 2.2 Proses Pembuatan Daemon
#### 1.  Fork Parent Process dan penghentian Parent Process 

Langkah pertama adalah men*spawn* proses menjadi induk dan anak dengan melakukan *forking*,  kemudian membunuh proses induk. Proses induk yang mati akan menyebabkan sistem operasi mengira bahwa proses telah selesai.

```c
pid_t pid, sid;
pid=fork();
if (pid < 0){
    exit(EXIT_FAILURE);
}
if (pid > 0){
    //catat PIP proses anak ke sebuah file
    exit(EXIT_SUCCESS);
}   //jika pembuatan proses berhasil, maka parent proses akan dimatikan
```

#### 2. Mengubah mode file menggunakan `umask(0)`

Untuk menulis beberapa file (termasuk logs) yang dibuat oleh daemon, mode file harus diubah untuk memastikan bahwa file tersebut dapat ditulis dan dibaca secara benar. Pengubahan mode file menggunakan implementasi umask().

#### 3. Membuat Unique Session ID (SID)

Child Process harus memiliki unik SID yang berasal dari kernel agar prosesnya dapat berjalan. Child Process menjadi Orphan Process pada system. Tipe pid_t juga digunakan untuk membuat SID baru untuk Child Process. Setsid() digunakan untuk pembuatan SID baru. Fungsi setsid() memiliki return tipe yang sama dengan fork().

```c
sid = setsid();
if(sid<0){
    exit(EXIT_FAILURE);
}  
```

#### 4. Mengubah Directory Kerja

Directori kerja yang aktif harus diubah ke suatu tempat yang telah pasti akan selalu ada. Pengubahan tempat direktori kerja dapat dilakukan dengan implementasi fungsi `chdir()`. Fungsi `chdir()` mengembalikan nilai -1 jika gagal.

```c
if((chdir("/"))<0) {
    exit(EXIT_FAILURE);
}
```

#### 5. Menutup File Descriptor Standar

Langkah terakhir dalam men-set daemon adalah menutup file desciptor standard dengan menggunakan STDIN, STDOUT, dan STDERR. Dikarenakan oleh daemon tidak menggunakan terminal, maka file desciptor dapat terus berulang dan berpotensi berbahaya bagi keamanan. Untuk mengatasi hal tersebut maka harus menggunakan fungsi close().

```c
close(STDIN_FILENO);
close(STDERR_FILENO);
clode(STDOUT_FILENO);
```

#### 6. Membuat Loop utama 

Daemon bekerja dalam jangka waktu tertentu, sehingga diperlukan sebuah looping.

```c
while(1){
    sleep(5)
}
exit(EXIT_SUCCES);
```
### 2.3 Contoh Implementasi Daemon
```C
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
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

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

  while(1) {
    // main program here
    sleep(30);
  }
  
  exit(EXIT_SUCCESS);
}
```

## Appendix
### Soal Latihan
#### Latihan 1
Modifikasi code [soal1](https://github.com/yoshimaputri/sisop-modul-2/blob/master/soal1.c) agar output nya menjadi huruf abjad urut dari A sampai Z, tanpa menghapus fungsi yang sudah ada.
#### Latihan 2
Buatlah program yang bisa membuat folder "anak" yang berisi salinan file [warisan.txt](https://github.com/yoshimaputri/sisop-modul-2/blob/master/warisan.txt).   
Hint: gunakan `fork`, `exec`, dan `wait`.
#### Latihan 3
Buatlah sebuah daemon yang dapat melakukan backup isi dari file **sampah.txt** yang disimpan dalam file **log.log** lalu menghapus file **sampah.txt** tersebut.
Tidak diperbolehkan menggunakan `exec` dan `system`.
### References
https://notes.shichao.io/apue/

http://www.linuxzasve.com/preuzimanje/TLCL-09.12.pdf