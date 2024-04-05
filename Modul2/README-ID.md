# Modul II - Proses, Thread, dan IPC

## Capaian

1. Memahami apa itu proses, thread, dan IPC
2. Mampu membedakan perbedaan proses dan thread
3. Dapat membuat program dengan multiproses dan multithread

## Daftar Isi

- [i. Capaian](#capaian)
- [ii. Daftar Isi](#daftar-isi)
- [1. Pendahuluan](#pendahuluan)
  - [1.1 Pengertian Proses](#pengertian-proses)
  - [1.2 Pengertian Thread](#pengertian-thread)
  - [1.3 Pengertian Multiprocess dan Multithread](#pengertian-multiprocess-dan-multithread)
- [2. Proses](#proses)
  - [2.1 Macam-Macam PID](#macam-macam-pid)
  - [2.2 Melihat Proses Berjalan](#melihat-proses-berjalan)
  - [2.3 Menghentikan Proses](#menghentikan-proses)
  - [2.4 Membuat Proses](#membuat-proses)
  - [2.5 Menjalankan Proses di Latar Belakang](#menjalankan-proses-di-latar-belakang)
  - [2.6 Menjalankan Proses Secara Bersamaan](#menjalankan-proses-secara-bersamaan)
  - [2.7 Jenis-Jenis Proses](#jenis-jenis-proses)
  - [2.8 Daemon](#daemon)
- [3. Thread](#thread)
  - [3.1 Perbedaan Multithread dan Multiprocess](#multiprocess-vs-multithread)
  - [3.2 Pembuatan Thread](#pembuatan-thread)
  - [3.3 Join Thread](#join-thread)
  - [3.4 Mutual Exclusion](#mutual-exclusion)
- [4. IPC](#ipc-interprocess-communication)
  - [4.1 Pengertian IPC](#ipc)
  - [4.2 Piping](#pipes)
  - [4.3 Shared Memory](#shared-memory)

</br></br>

## Pendahuluan

### Pengertian Proses

Pernahkah kalian membuka banyak aplikasi dalam laptop? Jika iya, maka kalian telah mengimplementasikan `proses`. Meskipun, kita sedang membuka satu aplikasi, tetapi aplikasi yang lain masih ada di latar belakang sebagai proses yang menunggu giliran.

![open-apps-on-laptop](img/many-apps-on-laptop.jpeg)

Proses sendiri dapat didefinisikan sebagai program yang sedang dieksekusi oleh OS. Ketika suatu program tersebut dieksekusi oleh OS, proses tersebut memiliki PID (Process ID) yang merupakan identifier dari suatu proses. Pada UNIX, untuk melihat proses yang dieksekusi oleh OS dengan memanggil perintah shell `ps`. Untuk melihat lebih lanjut mengenai perintah `ps` dapat membuka `man ps`.

### Pengertian Thread

Thread adalah unit dasar dari eksekusi yang dapat melakukan tugas-tugas tertentu di dalam sebuah proses. Thread-thread ini bekerja bersama-sama di dalam sebuah proses untuk menyelesaikan pekerjaan secara bersamaan. Mereka berbagi sumber daya dan konteks yang sama dengan proses utama di mana mereka berjalan.

![many-tabs-opened-at-once](img/tabs-opened-at-once.png)

Contoh dari thread adalah saat kita membuka browser, umumnya kita akan membuka banyak tab secara bersamaan. Masing-masing tab atau jendela tersebut mungkin akan dijalankan sebagai thread yang berbeda dalam satu proses utama dari aplikasi web browser.

### Pengertian Multiprocess dan Multithread

![multiprocess-multithread](img/multiprocess-multithread.png)

1. Multiprocess

Multiproses adalah pendekatan di mana sistem operasi dapat menjalankan beberapa proses secara bersamaan.

Karakteristik:

- Memiliki memori yang terpisah dan sumber daya yang terisolasi
- Proses-proses ini tidak berbagi memori atau variabel antara satu sama lain, kecuali jika ada mekanisme khusus seperti shared memory.
- Jika satu proses mengalami kegagalan atau crash, proses lainnya biasanya tidak terpengaruh.

Contoh kasus:

Saat membuka beberapa aplikasi dalam satu waktu, jika terdapat satu aplikasi yang bermasalah/crash, maka aplikasi lain tidak akan terpengaruh

2. Multithread

Multithreading adalah pendekatan di mana sebuah proses dapat memiliki beberapa thread yang berjalan secara bersamaan di dalamnya.

Karakteristik:

- Thread-thread dalam satu proses berbagi memori dan sumber daya. Mereka dapat saling berkomunikasi dengan mudah dan berbagi variabel.
- Thread-thread dapat melakukan tugas-tugas yang berbeda secara bersamaan dalam satu proses, meningkatkan efisiensi dan responsifitas.
- Jika satu thread mengalami kegagalan atau crash, hal itu dapat mempengaruhi keseluruhan proses dan thread-thread lainnya.

Contoh kasus:

Misalnya, sebuah server web perlu mampu menangani banyak permintaan HTTP dari klien secara bersamaan tanpa menghambat kinerja atau waktu tanggapan. Dibutuhkan multithreading untuk menangani setiap permintaan klien secara terpisah. Setiap kali server menerima permintaan baru, ia akan membuat thread baru untuk menangani permintaan tersebut.

</br>
</br>

## Proses

### Macam-Macam PID

#### User ID (UID)

Merupakan identifier dari suatu proses yang menampilkan user yang menjalankan suatu program. Pada program C, dapat memanggil fungsi ` uid_t getuid(void);`

#### Process ID (PID)

Angka unik dari suatu proses yang sedang berjalan untuk mengidentifikasi suatu proses. Pada program C, dapat memanggil fungsi `pid_t getpid(void);`

#### Parent PID (PPID)

Setiap proses memiliki identifier tersendiri dan juga setelah proses tersebut membuat proses lainnya. Proses yang terbentuk ini memiliki identifier berupa ID dari pembuatnya (parent). Pada program C, dapat memanggil fungsi `pid_t getppid(void); `.

### Melihat Proses Berjalan

Untuk melihat proces yang sedang berjalan di OS, dapat menggunakan `ps`.

![show ps](img/ps.jpg)

atau jika ingin lebih jelas dan detail dapat menggunakan command `ps aux`.

![show psaux](img/psaux.jpg)

perbedaan nya adalah jika hanya mengguanakan ps maka akan melihat proses hanya di user tersebut dan jika ps aux akan memperlihatkan semua proses yang berjalan meskipun di user lain.

Penjelasan:

- **UID**: user yang menjalankan program
- **PID**: process IDnya
- **PPID**: parent PID, kalau tidak ada parent akan bernilai 0
- **C**: CPU Util. (%)
- **STIME**: waktu proses dijalankan
- **TTY**: terminal yang menjalankan proses. Jika tidak ada berarti background
- **TIME**: lamanya proses berjalan
- **CMD**: perintah yang menjalankan proses tersebut

Kita juga dapat melihat proses yang berjalan dalam bentuk tree, sehingga kita dengan mudah mengidentifikasi mana yang merupakan child proses dan mana parent nya. Dengan mengguunakan command `pstree`

![show pstree](img/pstree.jpg)

### Menghentikan Proses

Untuk menghentikan (_terminate_) proses yang berjalan, jalankan perintah shell `kill [options] <pid>`. Biasanya untuk menghentikan paksa suatu proses dapat menggunakan perintah `kill -9 <pid>`. Angka _9_ adalah kode Signal untuk terminate suatu process.

Selain mengguankan command `kill` kita juga dapat menggunakan command `pkill`. Perbedaan penggunaannya jika menggunakan pkill adalah dengan menambahkan nama prosesnya `pkill [options] <nama proses>`

Kalian dapat melihat PID dan Nama proses menggunakan `jobs -l` atau `ps aux`

#### Macam-Macam Signal

| Signal name | Signal value | Effect                  |
| ----------- | :----------: | ----------------------- |
| SIGHUP      |      1       | Hangup                  |
| SIGINT      |      2       | Interrupt from keyboard |
| SIGKILL     |      9       | Kill signal             |
| SIGTERM     |      15      | Termination signal      |
| SIGSTOP     |   17,19,23   | Stop the process        |

Secara default ketika menggunakan perintah shell `kill <pid>`, akan menggunakan `SIGTERM` yang mana akan menghentikan proses namun masih dapat dilanjutkan kembali.

Dan jika kita mengguanakan `Ctrl + C` untuk menghentikkan suatu program , saat itu akan dikirmakan signal `SIGINT` yang mana proses akan dihentikan secara permanen oleh sistem.

Sedangkan jika kita menggunakan `Ctrl + Z` untuk menhentikan suatu program , saat itu sistem akan mengirimkan signal `SIGSTP` yang artinya menjeda proses tersebut dan dapat dijalankan kembali dengan menggunakan perintah `fg` atau `bg`.

### Membuat Proses

#### **fork**

`fork` adalah fungsi _system call_ di C untuk melakukan _spawning process_. Setelah memanggil fungsi itu, akan terdapat proses baru yang merupakan _child process_, fungsi akan mengembalikan nilai 0 di dalam _child process_, dan akan mengembalikan nilai _PID_ dari _child process_ di dalam _parent process_

Coba program dibawah ini dan compile terlebih dahulu dengan `gcc coba.c -o coba`

Kemudian execute program dengan `./coba`

Contoh program biasa tanpa menggunakan `fork()`

```c
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("Hello World!\n");
    return 0;
}
```

Contoh program dengan menggunakan `fork()`

```c
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
  pid_t child_id;

  child_id = fork();

  printf("Hello World\n");

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
Hello World

Parent process.
PID: 13101, Child's PID: 13102
Hello World

Child process.
PID: 13102, Parent's PID: 1
```

Hello World terpanggil dua kali, karena terpanggil kembali pada proses baru dan dengan PID yang berbeda.

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

#### **exec**

`exec` adalah fungsi untuk menjalankan program baru dan menggantikan program yang sedang berjalan. Fungsi `exec` memiliki banyak variasi seperti `execvp`, `execlp`, dan `execv`.

Contoh yang akan digunakan adalah `execv`.

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

### Menjalankan Proses di latar belakang

Dengan menggunakan `&` diakhir command kita dapat menjalankan program di latar belakang sehingga kita dapat melakukan hal lain sembari proses lain berjalan.

contoh implementasinya pada command wget dibawah:

![show background](img/background.jpg)

Dengan demikian kita tidak harus menunggu dan tetap bisa menggunakan terminal untuk keperluan yang lain. Dan jika ingin melihat progres dari proses tersebut bisa menggunakan command `jobs`.

Dan ini jika kita menjalankannya di foreground:

![show foreground](img/foreground.jpg)

Jika seperti ini maka proses akan berjalan secara foreground sehingga akan muncul di layar kalian.

### Menjalankan Proses Secara Bersamaan

Dengan menggabungkan `fork` dan `exec`, kita dapat melakukan dua atau lebih _tasks_ secara bersamaan. Contohnya adalah membackup log yang berbeda secara bersamaan.

```c
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main() {
  pid_t child_id;

  child_id = fork();
  char *username = getenv("USER");
  char destination[100];
  sprintf(destination, "/home/%s/", username);

  if (child_id < 0) {
    exit(EXIT_FAILURE); // Jika gagal membuat proses baru, program akan berhenti
  }

  if (child_id == 0) {
    // this is child

    char *argv[] = {"cp", "/var/log/apt/history.log", destination, NULL};
    execv("/bin/cp", argv);
  } else {
    // this is parent

    char *argv[] = {"cp", "/var/log/dpkg.log", destination, NULL};
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

Jika ingin melakukan banyak task secara bersamaan tanpa mementingkan urutan kerjanya, dapat menggunakan `fork` dan `exec`.

#### **wait** x **fork** x **exec**

Kita dapat menjalankan dua proses dalam satu program. Contoh penggunaannya adalah membuat folder dan mengisi folder tersebut dengan suatu file. Pertama, buat folder terlebih dahulu. Kemudian, buat file dengan perintah shell `touch` pada folder tersebut. Namun, pada kenyataannya untuk melakukan dua hal bersamaan perlu adanya jeda beberapa saat.

Untuk membuat file yang berada dalam suatu folder, pertama-tama folder harus ada terlebih dahulu. Untuk _delay_ suatu proses dapat menggunakan _system call_ `wait`.

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        printf("Error: Fork Failed\n");
        exit(1);
    }
    else if (pid == 0) {
        printf("Hello World from child process!\n");
        exit(0);
    }
    else {
        printf("Hello World from parent process!\n");
        int status;
        wait(&status);
        if (WIFEXITED(status)) {
            printf("Child process terminated normally with exit status: %d\n", WEXITSTATUS(status));
        }
        else {
            printf("Child process terminated abnormally\n");
        }
        exit(0);
    }
}
```

Pada contoh di atas, parent proses akan menunggu child proses selesai dijalankan dengan menggunakan `wait()` dan kemudian mengeluarkan pesan sesuai dengan status keluaran child proses menggunakan `WIFEXITED()` dan `WEXITSTATUS()`. Jika exit status = 0 menandakan program selesai dijalankan dengan benar.

Output Program

```
Hello from parent process!
Hello from child process!
Child process terminated normally with exit status: 0

```

Contoh lain :

```c
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

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

Pada contoh di atas, fungsi `wait` adalah menunggu _child process_ selesai melakukan tugasnya, yaitu membuat folder. Setelah _terminated_, _parent process_ akan kembali menjalankan prosesnya membuat `fileku` dalam folder `folderku`.

#### **system**

`system` adalah fungsi untuk melakukan pemanggilan perintah shell secara langsung dari program C. Contohnya ketika ingin memanggil suatu script dalam program C. `system(ls)` akan menghasilkan output yang sama ketika memanggilnya di shell script dengan `ls`.

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

### Jenis-Jenis Proses

#### **Zombie Process**

Zombie Process terjadi karena adaanya child process yang di exit namun parrent processnya tidak tahu bahwa child process tersebut telah di terminate, misalnya disebabkan karena putusnya network. Sehingga parent process tidak merelease process yang masih digunakan oleh child process tersebut walaupun process tersebut sudah mati. Dan proses ini tidak akan hilang sebelum komputer direstart atau dimatikan.

#### **Orphan Process**

Orphan Process adalah sebuah proses yang ada dalam komputer dimana parent process telah selesai atau berhenti bekerja namun proses anak sendiri tetap berjalan.

#### **Daemon Process**

Daemon Process adalah sebuah proses yang bekerja pada background karena proses ini tidak memiliki terminal pengontrol. Dalam sistem operasi Windows biasanya lebih dikenal dengan sebutan service. Daemon adalah sebuah proses yang didesain supaya proses tersebut tidak mendapatkan intervensi dari user.

### Pengertian Daemon

Daemon adalah suatu program yang berjalan di background secara terus menerus tanpa adanya interaksi secara langsung dengan user yang sedang aktif.

<!-- Sebuah daemon dapat berhenti karena beberapa hal. -->

### Langkah Pembuatan Daemon

Ada beberapa langkah untuk membuat sebuah daemon:

#### Melakukan Fork pada Parent Process dan mematikan Parent Process

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

#### Mengubah Mode File dengan `umask`

Setiap file dan directory memiliki _permission_ atau izin yang mengatur siapa saja yang boleh melakukan _read, write,_ dan _execute_ pada file atau directory tersebut.

Dengan menggunakan `umask` kita dapat mengatur _permission_ dari suatu file pada saat file itu dibuat. Di sini kita mengatur nilai `umask(0)` agar kita mendapatkan akses full terhadap file yang dibuat oleh daemon.

```c
umask(0);
```

#### Membuat Unique Session ID (SID)

Sebuah Child Process harus memiliki SID agar dapat berjalan. Tanpa adanya SID, Child Process yang Parent-nya sudah di-`kill` akan menjadi Orphan Process.

Untuk mendapatkan SID kita dapat menggunakan perintah `setsid()`. Perintah tersebut memiliki _return type_ yang sama dengan perintah `fork()`.

```c
sid = setsid();
if (sid < 0) {
  exit(EXIT_FAILURE);
}
```

#### Mengubah Working Directory

Working directory harus diubah ke suatu directory yang pasti ada. Untuk amannya, kita akan mengubahnya ke root (/) directory karena itu adalah directory yang dijamin ada pada semua distro linux.

Untuk mengubah Working Directory, kita dapat menggunakan perintah `chdir()`.

```c
if ((chdir("/")) < 0) {
  exit(EXIT_FAILURE);
}
```

#### Menutup File Descriptor Standar

Sebuah daemon tidak boleh menggunakan terminal. Oleh sebab itu kita harus _menutup_ file descriptor standar (STDIN, STDOUT, STDERR).

```c
close(STDIN_FILENO);
close(STDOUT_FILENO);
close(STDERR_FILENO);
```

File descriptor sendiri merupakan sebuah angka yang merepresentasikan sabuah file yang dibuka di sebuah sistem operasi. File descriptor mendeskripsikan sumber data dan bagaimana data itu diakses.

#### Membuat Loop Utama

Di loop utama ini lah tempat kita menuliskan inti dari program kita. Jangan lupa beri perintah `sleep()` agar loop berjalan pada suatu interval.

```c
while (1) {
  // Tulis program kalian di sini

  sleep(30);
}
```

</br>
</br>

## Thread

### Multiprocess Vs Multithread

![multivsmulti](img/multiprocessing_multithreading.gif)

Perbedaan _multiprocess_ dan _multithread_.

| Nomor | Multiprocess                                                        | Multithread                                                                                                                                                                                                                     |
| ----- | ------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| 1     | banyak proses dieksekusi secara konkuren                            | banyak thread dalam 1 proses dieksekusi secara konkuren                                                                                                                                                                         |
| 2     | menambah CPU untuk menigkatkan kekuatan komputasi                   | membuat banyak thread dalam 1 proses untuk meningkatkan kekuatan komputasi                                                                                                                                                      |
| 3     | pembuatan proses membutuhkan waktu dan resource yang besar          | pembuatan thread lebih ekonomis dalam segi waktu dan resource                                                                                                                                                                   |
| 4     | bergantung pada object di memori untuk mengirim data ke proses lain | tidak bergantung pada object lain                                                                                                                                                                                               |
| 5     | process sebagian besar bersifat interruptible / killable            | threading lebih susah untuk dikill atau diinterrup karena sebuah thread ada dalam sebuah proses sehingga jika ingin menginterrup thread harus melalui prosesnya (yang dikill prosesnya , otomatis thread akan juga terinterrup) |

- Contoh Penggunaan `Multi Processing` adalah pada sistem browser chrome, ketika kita membuka atau membuat tab baru maka sistem juga akan membuat process baru untuk kebutuhan tab baru tersebut sedangkan contoh implementasi `Multi Threading` adalah pada sistem sebuah game dimana sebuah proses dapat menangani berbagai kebutuhan secara bersamaan contohnya sebuah game dapat melakukan rendering beberapa objek bersamaan sehingga proses akan lebih cepat.

### Pembuatan Thread

Thread dapat dibuat menggunakan fungsi pada program berbahasa C sebagai berikut.

```c
#include <pthread.h> //library thread

int pthread_create(pthread_t *restrict tidp,
                   const pthread_attr_t *restrict attr,
                   void *(*start_rtn)(void *),
                   void *restrict arg);

/* Jika berhasil mengembalikan nilai 0, jika error mengembalikan nilai 1 */
```

Penjelasan syntax:

- Pointer `tidp` digunakan untuk menunjukkan alamat memori dengan thread ID dari thread baru.
- Argumen `attr` digunakan untuk menyesuaikan atribut yang digunakan oleh thread. nilai `attr` di-set `NULL` ketika thread menggunakan atribut _default_.
- Thread yang baru dibuat akan berjalan dimulai dari fungsi `start_rtn` dalam fungsi thread.
- Pointer `arg` digunakan untuk memberikan sebuah argumen ke fungsi `start_rtn`, jika tidak diperlukan argumen, maka `arg` akan di-set `NULL`.

Contoh Program menggunakan Thread

> compile dengan cara `gcc -pthread -o [output] input.c`

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *run(void *args) {
	int angka;
	angka = *((int *)args);

	if (angka == 1) {
		printf("%d bukan prima\n", angka);
		return NULL;
	}

	for (int i=2; i<angka; i++){
		if (angka % i == 0){
			printf("%d bukan prima\n", angka);
			return NULL;
		}
	}
	printf("%d prima\n", angka);
        return NULL;
}

void main() {
	int angka;
	printf("Masukkan angka: ");
	scanf("%d", &angka);

	pthread_t t_id[angka];
	printf("Thread berhasil dibuat\n");

	for (int i=0 ; i<angka; i++) {
		int *num_to_check = (int *)malloc(sizeof(int));
		*num_to_check = i+1 ;
		pthread_create(&t_id[i], NULL, &run, (void *)num_to_check);
	}

	for (int i=0 ; i<angka; i++) {
		pthread_join(t_id[i], NULL);
	}
	printf("Thread telah selesai\n");

}
```

Program Pembanding jika tidak menggunakan _Thread_.

```c
#include <stdio.h>
#include <stdlib.h>

int is_prime(int angka) {
    if (angka == 1) {
        printf("%d bukan prima\n", angka);
        return 0;
    }

    for (int i=2; i<angka; i++){
        if (angka % i == 0){
            printf("%d bukan prima\n", angka);
            return 0;
        }
    }
    printf("%d prima\n", angka);
    return 1;
}

int main() {
    int jumlah;
    printf("Masukkan jumlah angka: ");
    scanf("%d", &jumlah);

    printf("Hasil pengujian bilangan:\n");
    for (int i=0; i<jumlah; i++) {
        is_prime(i);
    }

    return 0;
}
```

Perbandingan antara Thread dengan Fork.

Contoh membuat program tanpa menggunakan thread:

```c
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>

int main()
{
	pid_t child;
	int i, stat;
	char *argv1[] = {"clear", NULL};
	char *argv2[] = {"xlogo", NULL};
	child = fork();
	if (child==0) {
		execv("/usr/bin/clear", argv1);
	}
	else
	{
		for(i=0;i<6;i++)
		{
			printf("%d\n",i);
			fflush(stdout);
			sleep(1);
		}
		execv("/usr/bin/xlogo", argv2);
	}

}
```

Contoh membuat program menggunakan thread.

> compile dengan cara `gcc -pthread -o [output] input.c`

```c
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>

pthread_t tid[3]; //inisialisasi array untuk menampung thread dalam kasus ini ada 2 thread
pid_t child;

int length=5; //inisialisasi jumlah untuk looping
void* playandcount(void *arg)
{
	char *argv1[] = {"clear", NULL};
	char *argv2[] = {"xlogo", NULL};
	unsigned long i=0;
	pthread_t id=pthread_self();
	int iter;
	if(pthread_equal(id,tid[0])) //thread untuk clear layar
	{
		child = fork();
		if (child==0) {
		    execv("/usr/bin/clear", argv1);
	    	}
	}
	else if(pthread_equal(id,tid[1])) // thread menampilkan counter
	{
        for(iter=0;iter<6;iter++)
		{
			printf("%d\n",iter);
			fflush(stdout);
			sleep(1);
		}
	}
	else if(pthread_equal(id,tid[2])) // thread menampilkan gambar
	{
        child = fork();
        if (child==0) {
		    execv("/usr/bin/xlogo", argv2);
	    }
	}

	return NULL;
}

int main(void)
{
	int i=0;
	int err;
	while(i<3) // loop sejumlah thread
	{
		err=pthread_create(&(tid[i]),NULL,&playandcount,NULL); //membuat thread
		if(err!=0) //cek error
		{
			printf("\n can't create thread : [%s]",strerror(err));
		}
		else
		{
			printf("\n create thread success\n");
		}
		i++;
	}
	pthread_join(tid[0],NULL);
	pthread_join(tid[1],NULL);
	exit(0);
	return 0;
}
```

**Kesimpulan** :
Terlihat ketika program menggunakan thread dapat menjalankan dua task secara bersamaan dan konsumsi cpu lebih kecil jika dibanding dengan create suaru proses baru.

### Join Thread

Join thread adalah fungsi untuk melakukan penggabungan dengan thread lain yang telah berhenti (_terminated_). Bila thread yang ingin di-join belum dihentikan, maka fungsi ini akan menunggu hingga thread yang diinginkan berstatus **`Terminated`**. Fungsi `pthread_join()` ini dapat dikatakan sebagai fungsi `wait()` pada proses, karena program (_task_) utama akan menunggu thread yang di-join-kan pada program utama tersebut. Kita tidak mengetahui program utama atau thread yang lebih dahulu menyelesaikan pekerjaannya.

Contoh program C Join_Thread:

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h> //library thread

void *print_message_function( void *ptr );

int main()
{
    pthread_t thread1, thread2;//inisialisasi awal
    const char *message1 = "Thread 1";
    const char *message2 = "Thread 2";
    int  iret1, iret2;

    iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1); //membuat thread pertama
    if(iret1) //jika eror
    {
        fprintf(stderr,"Error - pthread_create() return code: %d\n",iret1);
        exit(EXIT_FAILURE);
    }

    iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);//membuat thread kedua
    if(iret2)//jika gagal
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

<details>
  <summary>Keterangan & Kesimpulan</summary>

- Pada program di atas, jika kita _comment_ baris `pthread_join`, maka hasil yang didapat tidak akan memunculkan tulisan **Thread 1** dan **Thread 2**.
- Jika pemanggilan fungsi `pthread_join` di-uncomment, maka program yang kita buat akan memunculkan tulisan **Thread 1** dan **Thread 2**.

Pada program pertama tidak menjalankan fungsi `print_message_function` karena sebelum kedua thread dijadwalkan, program utama (kemungkinan) telah selesai dieksekusi sehingga tidak menjalankan fungsi bawaan pada thread. Pada percobaan kedua, fungsi `pthread_join()` digunakan untuk membuat program utama menunggu thread yang _join_ hingga target thread selesai dieksekusi, dengan fungsi ini program utama di-suspend hingga target thread selesai dieksekusi.

- Fungsi untuk terminasi thread
  ```c
  #include <pthread.h>
  void pthread_exit(void *rval_ptr);
  ```
  Argumen `rval_ptr` adalah pointer yang digunakan yang dapat diakses oleh fungsi `pthread_join()` agar dapat mengetahui status thread tersebut
- Fungsi untuk melakukan join thread
  ```c
  int pthread_join(pthread_t thread, void **rval_ptr);
  /* Jika berhasil mengembalikan nilai 0, jika error mengembalikan nilai 1 */
  ```
  Fungsi akan menunda pekerjaan sampai status pointer `rval_ptr` dari fungsi `pthread_exit()` mengembalikan nilainya.
  </details>

### Mutual Exclusion

Disebut juga sebagai **Mutex**, yaitu suatu cara yang menjamin jika ada pekerjaan yang menggunakan variabel atau berkas digunakan juga oleh pekerjaan yang lain, maka pekerjaan lain tersebut akan mengeluarkan nilai dari pekerjaan sebelumnya.

Contoh program Simple Mutual_Exclusion yang mana di sini menggunakan flag `status`.

```c
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
pthread_t tid1, tid2;
int status;
int nomor;
void* tulis(void *arg)
{
    status = 0;
    printf("Masukan nomor : ");
    scanf("%d", &nomor);
    status = 1;
    return NULL;
}
void* baca(void *arg)
{
    while(status != 1)
    {
    }
    printf("Nomor %d\n", nomor);
}
int main(void)
{
    pthread_create(&(tid1), NULL, tulis, NULL);
    pthread_create(&(tid2), NULL, baca, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
```

Keterangan :

- Terdapat 2 buah thread yang berjalan dengan fungsi yang berbeda.
- Sumber daya (variabel) yang digunakan kedua thread untuk mengeksekusi pekerjaannya **sama**.
- Variabel `status` adalah contoh simple untuk mengendalikan jalannya thread.

Kemudian kita juga fungsi `pthread_mutex` yang telah disediakan oleh library `pthread.h`. Berikut contoh programnya:

```c
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
pthread_t tid[2];
int counter;
// lock: Variabel mutex yang digunakan untuk mengatur akses terhadap variabel counter.
pthread_mutex_t lock;
void* trythis(void* arg)
{
    //pthread_mutex_lock() digunakan untuk mengunci mutex lock, sehingga menghindari akses bersama pada variabel counter.
    pthread_mutex_lock(&lock);
    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d has started\n", counter);
    for (i = 0; i < (0xFFFFFFFF); i++)
        ;
    printf("\n Job %d has finished\n", counter);
    //pthread_mutex_unlock() digunakan untuk membuka kunci mutex lock agar memungkinkan akses dari thread-thread lain.
    pthread_mutex_unlock(&lock);
    return NULL;
}
int main(void)
{
    int i = 0;
    int error;
    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("\n mutex init has failed\n");
        return 1;
    }
    while (i < 2) {
        error = pthread_create(&(tid[i]),
        NULL,
        &trythis, NULL);
        if (error != 0)
            printf("\nThread can't be created :[%s]",
            strerror(error));
        i++;
    }
    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}
```

Output :

```
Job 1 has started
Job 1 has finished
Job 2 has started
Job 2 has finished
```

**Kesimpulan** :
Karena kita tidak mengetahui _thread_ mana yang lebih dahulu mengeksekusi sebuah variable atau sumber daya pada program, kegunaan dari **Mutex** adalah untuk menjaga sumber daya suatu thread agar tidak digunakan oleh thread lain sebelum ia menyelesaikan pekerjaannya.

</br></br>

## IPC (Interprocess Communication)

### IPC

IPC (_Interprocess Communication_) adalah cara atau mekanisme pertukaran data antara satu proses dengan proses lain, baik pada komputer yang sama atau komputer jarak jauh yang terhubung melalui suatu jaringan.

### Pipes

_Pipe_ merupakan komunikasi sequensial antar proses yang saling terelasi. Kelemahannya, hanya dapat digunakan untuk proses yang saling berhubungan dan secara sequensial.

Terdapat dua jenis _pipe_ sebagai berikut.

- `unnamed pipe`: Komunikasi antara _parent_ dan _child_ proses.
- `named pipe`: Biasa disebut sebagai FIFO, digunakan untuk komunikasi yang berjalan secara independen. **Hanya bisa digunakan jika kedua proses menggunakan _filesystem_ yang sama.**

```
$ ls | less
```

Diagram dari _pipe_ dapat ditunjukkan sebagai berikut.

![alt](img/pipe.png "Diagram Pipe")

<!-- TAMBAHKAN PENJELASAN TENTANG FDS (FILE DESCRIPTOR). SAMA KAYA PID, TAPI BUAT PIPING -->

_pseudocode_ dari _pipe_ (tanpa _fork_) dapat ditunjukkan sebagai berikut.

```pascal
int pipe(int fds[2]);

Parameters :
fd[0] will be the fd(file descriptor) for the
read end of pipe.
fd[1] will be the fd for the write end of pipe.
Returns : 0 on Success.
-1 on error.
```

Contoh kode dalam bahasa C (tanpa _fork_) dapat dilihat di bawah ini.

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

Output dari kode tersebut adalah seperti berikut.

```
hello, world #1
hello, world #2
hello, world #3
```

Diagram dari _pipe_ (dengan _fork_) dapat ditunjukkan sebagai berikut.

![alt](img/pipe-fork.png)

Contoh kode dalam bahasa C (dengan _fork_) dapat dilihat di bawah ini.

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

### Message Queues

Message queue merupakan suatu mekanisme _interprocess communication (IPC)_ yang memungkinkan suatu proses untuk melakukan pertukaran data berupa pesan diantara dua proses. Mekanisme ini memungkinkan proses untuk berkomunikasi secara asinkron dengan mengirim pesan satu sama lain. Pesan yang dikirim akan disimpan ke dalam suatu antrian, menunggu untuk diproses, kemudian dihapus setelah proses selesai berjalan.

Ilustrasi:

![ilustrasi-message-queue](https://static.javatpoint.com/operating-system/images/ipc-using-message-queues.png)

Message queue menggunakan prinsip FIFO (First In First Out) tidak terbatas yang tidak dapat diakses oleh dua thread yang berbeda. Dalam melakukan write pesan, banyak tasks dapat menulis pesan ke dalam queue, tetapi hanya satu tasks yang dapat membaca pesan secara sekaligus dari sebuah queue. Pembaca akan menunggu antrian pesan sampai ada pesan yang akan diproses.

Contoh program dapat diakses di [sender](playground/sender.c) dan [receiver](playground/receiver.c).

Contoh code pada sender:

```c
// C Program for Message Queue (Writer Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#define MAX 10

// structure for message queue
struct mesg_buffer {
	long mesg_type;
	char mesg_text[100];
} message;

int main()
{
	key_t key;
	int msgid;

	// ftok to generate unique key
	key = ftok("progfile", 65);

	// msgget creates a message queue
	// and returns identifier
	msgid = msgget(key, 0666 | IPC_CREAT);
	message.mesg_type = 1;

	printf("Write Data : ");
	fgets(message.mesg_text,MAX,stdin);

	// msgsnd to send message
	msgsnd(msgid, &message, sizeof(message), 0);

	// display the message
	printf("Data send is : %s \n", message.mesg_text);

	return 0;
}
```

Contoh code pada receiver:

```c
// C Program for Message Queue (Reader Process)
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// structure for message queue
struct mesg_buffer {
	long mesg_type;
	char mesg_text[100];
} message;

int main()
{
	key_t key;
	int msgid;

	// ftok to generate unique key
	key = ftok("progfile", 65);

	// msgget creates a message queue
	// and returns identifier
	msgid = msgget(key, 0666 | IPC_CREAT);

	// msgrcv to receive message
	msgrcv(msgid, &message, sizeof(message), 1, 0);

	// display the message
	printf("Data Received is : %s \n",
					message.mesg_text);

	// to destroy the message queue
	msgctl(msgid, IPC_RMID, NULL);

	return 0;
}

```

### Shared Memory

Sebuah mekanisme _mapping area (segments)_ dari suatu blok _memory_ untuk digunakan bersama oleh beberapa proses. Sebuah proses akan menciptakan _segment memory_, kemudian proses lain yang diijinkan dapat mengakses _memory_ tersebut. _Shared memory_ merupakan cara yang efektif untuk melakukan pertukaran data antar program. Dalam hal ini, apabila suatu proses melakukan perubahan, maka proses lain dapat melihatnya.

![shared-memory](https://static.javatpoint.com/operating-system/images/ipc-through-shared-memory.png)

Shared memory merupakan mekanisme IPC yang paling cepat. Suatu sistem operasi akan memetakan memory segment pada suatu address space dari beberapa proces untuk melakukan read and write di segmen memori tersebut tanpa memanggil fungsi dari sistem operasi. Shared memory ini merupakan mekanisme yang superior untuk melakukan pertukaran data dengan ukuran sangat besar.

Langkah-langkah menggunakan shared memory:

1. Melakukan request memory segment pada operating system yang bisa digunakan secara bersamaan oleh suatu proses
2. Melakukan asosiasi dari sebagian atau seluruh memory dengan address space dari proses yang dimaksud.

Ilustrasi
![shared-mem](https://static.javatpoint.com/operating-system/images/ipc-through-shared-memory2.png)

- Sebagai catatan, alamat memory dari suatu shared memory pada masing-masing proses belum tentu sama. Dalam hal ini, kita dapat menggunakan semaphore untuk melakukan sinkronisasi.

  
Example: [Proses 1](playground/shared-memory-1.c) [Proses 2](playground/shared-memory-2.c)

  
**Proses 1**

```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void main()
{
        key_t key = 1234;
        int *value;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

        *value = 10;

        printf("Program 1 : %d\n", *value);

        sleep(5);

        printf("Program 1: %d\n", *value);
        shmdt(value);
        shmctl(shmid, IPC_RMID, NULL);
}
```

**Proses 2**

```c
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

void main()
{
        key_t key = 1234;
        int *value;

        int shmid = shmget(key, sizeof(int), IPC_CREAT | 0666);
        value = shmat(shmid, NULL, 0);

        printf("Program 1 : %d\n", *value);
	*value = 30;

        sleep(5);

        printf("Program 1: %d\n", *value);
        shmdt(value);
        shmctl(shmid, IPC_RMID, NULL);
}
```

Jalankan proses 1 terlebih dahulu, lalu proses 2. Hasilnya adalah sebagai berikut.

**Proses 1**

```
Program 1 : 10
Program 1 : 30
```

**Proses 2**

```
Program 1 : 10
Program 1 : 30
```

</br></br>
