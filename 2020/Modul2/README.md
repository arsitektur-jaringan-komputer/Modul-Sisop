# sisop-modul-2

# Daemon dan Proses

Menggunakan:
* Linux
* Bahasa C (compile dengan _gcc_)

# Daftar Isi

* Proses dan Daemon
    * [Daftar Isi](#daftar-isi)
    * [Proses](#proses)
        * [1. Pengertian](#1-pengertian)
        * [2. Macam-Macam PID](#2-macam-macam-pid)
            * [2.1 User ID (UID)](#21-user-id-uid)
            * [2.2 Process ID (PID)](#22-process-id-pid)
            * [2.3 Parent PID (PPID)](#23-parent-pid-ppid)
        * [3. Melihat Proses Berjalan](#3-melihat-proses-berjalan)
        * [4. Menghentikan Proses](#4-menghentikan-proses)
        * [5. Membuat Proses](#5-membuat-proses)
            * [fork](#fork)
            * [exec](#exec)
            * [Menjalankan Program Secara Bersamaan](#menjalankan-program-secara-bersamaan)
            * [wait x fork x exec](#wait-x-fork-x-exec)
            * [system](#system)
        * [6. Jenis-Jenis Proses](#6-jenis-jenis-proses)
            * [Zombie Process](#zombie-process)
            * [Orphan Process](#orphan-process)
            * [Daemon Process](#daemon-process)
    * [Daemon](#daemon)
        * [1. Pengertian Daemon](#1-pengertian-daemon)
        * [2. Langkah Pembuatan Daemon](#2-langkah-pembuatan-daemon)
        * [3. Implementasi Daemon](#3-implementasi-daemon)
    * [Soal Latihan](#soal-latihan)

# Proses

## 1. Pengertian

[Daftar Isi](#daftar-isi)

Proses adalah kondisi dimana OS menjalankan (eksekusi) suatu program. Ketika suatu program tersebut dieksekusi oleh OS, proses tersebut memiliki PID (Process ID) yang merupakan identifier dari suatu proses. Pada UNIX, untuk melihat proses yang dieksekusi oleh OS dengan memanggil perintah shell ```ps```. Untuk melihat lebih lanjut mengenai perintah ```ps``` dapat membuka ```man ps```.

Dalam penggunaannya, suatu proses dapat membentuk proses lainnya yang disebut _spawning process_. Proses yang memanggil proses lainnya disebut **_parent process_** dan yang terpanggil disebut **_child process_**.

## 2. Macam-Macam PID

[Daftar Isi](#daftar-isi)

### 2.1 User ID (UID)
Merupakan identifier dari suatu proses yang menampilkan user yang menjalankan suatu program. Pada program C, dapat memanggil fungsi ``` uid_t getuid(void);```

### 2.2 Process ID (PID)
Angka unik dari suatu proses yang sedang berjalan untuk mengidentifikasi suatu proses. Pada program C, dapat memanggil fungsi ```pid_t getpid(void);```

### 2.3 Parent PID (PPID)
Setiap proses memiliki identifier tersendiri dan juga setelah proses tersebut membuat proses lainnya. Proses yang terbentuk ini memiliki identifier berupa ID dari pembuatnya (parent). Pada program C, dapat memanggil fungsi ```
pid_t getppid(void);```.

## 3. Melihat Proses Berjalan

[Daftar Isi](#daftar-isi)

Untuk melihat proces yang sedang berjalan di OS, dapat menggunakan ```ps -ef``` untuk melihat secara detailnya.

![show ps](img/showps.png)

Penjelasan:
  * **UID**: user yang menjalankan program
  * **PID**: process IDnya
  * **PPID**: parent PID, kalau tidak ada parent akan bernilai 0
  * **C**: CPU Util. (%)
  * **STIME**: waktu proses dijalankan
  * **TTY**: terminal yang menjalankan proses. Jika tidak ada berarti background
  * **TIME**: lamanya proses berjalan
  * **CMD**: perintah yang menjalankan proses tersebut

## 4. Menghentikan Proses

[Daftar Isi](#daftar-isi)

Untuk menghentikan (_terminate_) proses yang berjalan, jalankan perintah shell ```kill [options] <pid>```. Biasanya untuk menghentikan paksa suatu proses dapat menggunakan perintah ```kill -9 <pid>```. Angka _9_ adalah kode Signal untuk terminate suatu process.

### Macam-Macam Signal

| Signal name | Signal value  | Effect       |
| ------------|:-------------:| -------------|
| SIGHUP      | 1             | Hangup         |
| SIGINT      | 2             | Interrupt from keyboard  |
| SIGKILL     | 9             | Kill signal   |
| SIGTERM     | 15            | Termination signal
| SIGSTOP     | 17,19,23      | Stop the process

Secara default ketika menggunakan perintah shell ```kill <pid>```, akan menggunakan ```SIGSTOP``` yang mana akan menghentikan proses namun masih dapat dilanjutkan kembali.

## 5. Membuat Proses

[Daftar Isi](#daftar-isi)

### **fork**
```fork``` adalah fungsi _system call_ di C untuk melakukan _spawning process_. Setelah memanggil fungsi itu, akan terdapat proses baru yang merupakan _child process_ dan mengembalikan nilai 0 untuk _child process_ dan nilai _PID_ untuk _parent process_. 

Coba program dibawah ini dan compile terlebih dahulu dengan ```gcc coba.c -o coba```

Kemudian execute program dengan ```./coba```

```c
#include <stdio.h> 
#include <sys/types.h> 
#include <unistd.h> 

int main(){
  pid_t child_id;

  child_id = fork();

  printf("Ini akan kepanggil 2 kali\n");
  
  if(child_id != 0){
    printf("\nParent process.\nPID: %d, Child's PID: %d\n", (int)getpid(), (int)child_id);
  }else {
    printf("\nChild process.\nPID: %d, Parent's PID: %d\n", (int)getpid(), (int)getppid());
  }

  return 0;
}
```

Hasilnya akan menjadi:
```c
Ini akan kepanggil 2 kali

Parent process.
PID: 13101, Child's PID: 13102
Ini akan kepanggil 2 kali

Child process.
PID: 13102, Parent's PID: 1
```

Visualisasi:
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

```exec``` adalah fungsi untuk menjalankan program baru dan menggantikan program yang sedang berjalan. Fungsi ```exec``` memiliki banyak variasi seperti ```execvp```, ```execlp```, dan ```execv```.

Contoh yang akan digunakan adalah ```execv```.

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

### **Menjalankan Program Secara Bersamaan**

Dengan menggabungkan ```fork``` dan ```exec```, kita dapat melakukan dua atau lebih _tasks_ secara bersamaan. Contohnya adalah membackup log yang berbeda secara bersamaan.

```c
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
  pid_t child_id;

  child_id = fork();
  
  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
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

Visualisasi:
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


Jika ingin melakukan banyak task secara bersamaan tanpa mementingkan urutan kerjanya, dapat menggunakan ```fork``` dan ```exec```.

### **wait** x **fork** x **exec**

Kita dapat menjalankan dua proses dalam satu program. Contoh penggunaannya adalah membuat folder dan mengisi folder tersebut dengan suatu file. Pertama, buat folder terlebih dahulu. Kemudian, buat file dengan perintah shell ```touch``` pada folder tersebut. Namun, pada kenyataannya untuk melakukan dua hal bersamaan perlu adanya jeda beberapa saat. 

Untuk membuat file yang berada dalam suatu folder, pertama-tama folder harus ada terlebih dahulu. Untuk _delay_ suatu proses dapat menggunakan _system call_ ```wait```.

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
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
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

Pada contoh di atas, fungsi ```wait``` adalah menunggu _child process_ selesai melakukan tugasnya, yaitu membuat folder. Setelah _terminated_, _parent process_ akan kembali menjalankan prosesnya membuat ```fileku``` dalam folder ```folderku```.

### **system**

```system``` adalah fungsi untuk melakukan pemanggilan perintah shell secara langsung dari program C. Contohnya ketika ingin memanggil suatu script dalam program C. ```system(ls)``` akan menghasilkan output yang sama ketika memanggilnya di shell script dengan ```ls```.

File inibash.sh:
```sh
#!/bin/bash

echo "Shell script dipanggil"

```

File system.c:
```c
#include <stdlib.h>

int main() {
  int return_value;
  return_value = system("bash inibash.sh");
  return return_value;
}

```

Output:
```
Shell script dipanggil
```


## 6. Jenis-Jenis Proses

[Daftar Isi](#daftar-isi)

### **Zombie Process**

Zombie Process terjadi karena adaanya child process yang di exit namun parrent processnya tidak tahu bahwa child process tersebut telah di terminate, misalnya disebabkan karena putusnya network. Sehingga parent process tidak merelease process yang masih digunakan oleh child process tersebut walaupun process tersebut sudah mati.

### **Orphan Process**

Orphan Process adalah sebuah proses yang ada dalam komputer dimana parent process telah selesai atau berhenti bekerja namun proses anak sendiri tetap berjalan.

### **Daemon Process**

Daemon Process adalah sebuah proses yang bekerja pada background karena proses ini tidak memiliki terminal pengontrol. Dalam sistem operasi Windows biasanya lebih dikenal dengan sebutan service. Daemon adalah sebuah proses yang didesain supaya proses tersebut tidak mendapatkan intervensi dari user.

---

# Daemon

[Daftar Isi](#daftar-isi)

## 1. Pengertian Daemon
Daemon adalah suatu program yang berjalan di background secara terus menerus tanpa adanya interaksi secara langsung dengan user yang sedang aktif.

<!-- Sebuah daemon dapat berhenti karena beberapa hal. -->
## 2. Langkah Pembuatan Daemon
Ada beberapa langkah untuk membuat sebuah daemon:
### 2.1 Melakukan Fork pada Parent Process dan mematikan Parent Process
Langkah pertama adalah membuat sebuah parent process dan memunculkan child process dengan melakukan `fork()`. Kemudian bunuh parent process agar sistem operasi mengira bahwa proses telah selesai.

```c
pid_t pid;        // Variabel untuk menyimpan PID

pid = fork();     // Menyimpan PID dari Child Process

/* Keluar saat fork gagal
 * (nilai variabel pid < 0) */
if (pid < 0) {
  exit(EXIT_FAILURE);
}

/* Keluar saat fork berhasil
 * (nilai variabel pid adalah PID dari child process) */
if (pid > 0) {
  exit(EXIT_SUCCESS);
}
```

### 2.2 Mengubah Mode File dengan `umask`
Setiap file dan directory memiliki _permission_ atau izin yang mengatur siapa saja yang boleh melakukan _read, write,_ dan _execute_ pada file atau directory tersebut.

Dengan menggunakan `umask` kita dapat mengatur _permission_ dari suatu file pada saat file itu dibuat. Di sini kita mengatur nilai `umask(0)` agar kita mendapatkan akses full terhadap file yang dibuat oleh daemon.

```c
umask(0);
```

### 2.3 Membuat Unique Session ID (SID)
Sebuah Child Process harus memiliki SID agar dapat berjalan. Tanpa adanya SID, Child Process yang Parent-nya sudah di-`kill` akan menjadi Orphan Process.

Untuk mendapatkan SID kita dapat menggunakan perintah `setsid()`. Perintah tersebut memiliki _return type_ yang sama dengan perintah `fork()`.

```c
sid = setsid();
if (sid < 0) {
  exit(EXIT_FAILURE);
}
```

### 2.4 Mengubah Working Directory
Working directory harus diubah ke suatu directory yang pasti ada. Untuk amannya, kita akan mengubahnya ke root (/) directory karena itu adalah directory yang dijamin ada pada semua distro linux.

Untuk mengubah Working Directory, kita dapat menggunakan perintah `chdir()`.

```c
if ((chdir("/")) < 0) {
  exit(EXIT_FAILURE);
}
```

### 2.5 Menutup File Descriptor Standar
Sebuah daemon tidak boleh menggunakan terminal. Oleh sebab itu kita harus _menutup_ file descriptor standar (STDIN, STDOUT, STDERR).

```c
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
```

### 2.6 Membuat Loop Utama
Di loop utama ini lah tempat kita menuliskan inti dari program kita. Jangan lupa beri perintah `sleep()` agar loop berjalan pada suatu interval.

```c
while (1) {
  // Tulis program kalian di sini

  sleep(30);
}
```

## 3. Implementasi Daemon
Di bawah ini adalah kode hasil gabungan dari langkah-langkah pembuatan daemon (template Daemon):

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
  pid_t pid, sid;        // Variabel untuk menyimpan PID

  pid = fork();     // Menyimpan PID dari Child Process

  /* Keluar saat fork gagal
  * (nilai variabel pid < 0) */
  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  /* Keluar saat fork berhasil
  * (nilai variabel pid adalah PID dari child process) */
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
    // Tulis program kalian di sini

    sleep(30);
  }
}
```
### 3.1 Meng-_compile_ program daemon
Untuk menjalankan daemon process pertama kita compile program C yang telah kita buat dengan perintah `gcc [nama_program.c] -o [nama_file_outputd]`.

### 3.2 Menjalankan program daemon
Setelah melakukan langkah sebelumnya, akan muncul sebuah file executeable yang dapat dijalankan dengan `./nama_file_outputd`.

### 3.3 Periksa apakah Daemon process berjalan
Untuk memeriksa process apa saja yang sedang berlangsung kita dapat menggunakan perintah `ps -aux`. Untuk menemukan Daemon process yang kita _run_, manfaatkan `grep`. Sehingga perintahnya menjadi `ps -aux | grep "nama_file_outputd"`. Bila ada, berarti daemon process kita sedang berjalan.

### 3.4 Mematikan Daemon process yang sedang berjalan
Untuk mematikan daemon process kita akan menggunakan perintah `kill`. Pertama kita harus menemukan PID dari Daemon process yang akan dimatikan. Kita dapat menemukan PID tersebut pada langkah sebelumnya. Lalu jalankan `sudo kill -9 pid` untuk mematikan process-nya.

# Soal Latihan

[Daftar Isi](#daftar-isi)

### Latihan 1
Modifikasi code [soal1](https://github.com/AZakyH/sisop-modul-2/blob/master/soal1.c) agar output nya menjadi angka urut dari 0 sampai 100, tanpa menghapus fungsi yang sudah ada dan menggunakan **wait**.

### Latihan 2
Buatlah sebuah program yang dapat mengcopy folder beserta semua isi dari folder di */home/{USER}/Music* ke dalam sebuah folder dengan format nama *tanggal-bulan-tahun_jam:menit:detik* (contoh: 25-02-2020_16:37:53). **Gunakan fork, exec, dan wait.**

### Latihan 3
Buatlah sebuah daemon yang berjalan setiap 10 detik yang dapat melakukan backup isi dari file *diary.txt* yang disimpan dalam file *diary.log.{no}* (contoh: diary.log.1 , diary.log.2, … ) lalu menghapus isi *diary.txt* tersebut sehingga file tersebut kosong kembali. **Tidak diperbolehkan menggunakan exec dan system.**


## Referensi
* https://notes.shichao.io/apue/ch8/
* https://www.geeksforgeeks.org/exec-family-of-functions-in-c/
* http://www.netzmafia.de/skripten/unix/linux-daemon-howto.html
* https://www.computerhope.com/unix/uumask.htm
