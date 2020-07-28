
# File System dan FUSE

  

## Objectives

1. Peserta mengetahui _file system_

2. Peserta mengetahui dan mampu mengimplementasikan FUSE

  

## Daftar Isi

-  [1. File System](https://github.com/Armunz/sisop-modul-4#1-file-system)

-  [1.1 Tipe File System](https://github.com/Armunz/sisop-modul-4#11-tipe-file-system)

-  [1.2 Virtual File System](https://github.com/Armunz/sisop-modul-4#12-virtual-file-system)

-  [1.3 Dentry](https://github.com/Armunz/sisop-modul-4#13-dentry)

-  [1.4 Superblock](https://github.com/Armunz/sisop-modul-4#14-superblock)

-  [1.5 Inode](https://github.com/Armunz/sisop-modul-4#15-inode)

-  [2. File System in Userspace (FUSE)](https://github.com/Armunz/sisop-modul-4#2-file-system-in-userspace-fuse)

-  [2.1 Instalasi FUSE](https://github.com/Armunz/sisop-modul-4#21-instalasi-fuse)

-  [2.2 Cara Kerja FUSE](https://github.com/Armunz/sisop-modul-4#22-cara-kerja-fuse)

-  [2.3 Membuat Program FUSE](https://github.com/Armunz/sisop-modul-4#23-membuat-program-fuse)

- [2.4 Unmount FUSE](https://github.com/Armunz/sisop-modul-4#24-unmount-fuse)

  

# 1. File System

_File system_ adalah struktur logika yang digunakan untuk mengendalikan akses data seperti bagaimana dia disimpan maupun diambil. _File system_ sendiri memiliki banyak jenis dengan penggunaan algoritma yang tentu berbeda. Setiap Sistem Operasi (OS) memiliki support file system yang berbeda-beda. File system digunakan untuk mengorganisir dan menyimpan file pada storage device. 

_File system_ menyediakan cara untuk memisah-misahkan data pada drive menjadi bentuk tunggal yaitu file. _File system_ juga menyediakan cara untuk menyimpan data pada file, contohnya **filename**, **permission**, dan atribut lainnya. Pada File System, disediakan juga sebuah **index** yang berisi daftar file yang terletak pada suatu lokasi penyimpanan, sehingga Sistem Operasi dapat melihat ada apa saja pada lokasi penyimpanan tersebut.




![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/linux-filesystem.png?raw=true)
  

### 1.1 Tipe File System

**1. File System Disk**

_File system disk_ adalah _file system_ yang didesain untuk menyimpan data pada sebuah media penyimpan data. Contohnya: FAT (FAT 12, FAT 16, FAT 320), NTFS, HFS, HFS+, ext2, ext3, ext4, ISO 9660, ODS-5 dan UDF.

 - FAT32 dan NTFS adalah File System di Windows.

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/xformat-file-system-in-windows-disk-utility.png.pagespeed.gp+jp+jw+pj+ws+js+rj+rp+rw+ri+cp+md.ic._Lfdba02dY.png?raw=true)

 - Ext2, Ext3, Ext4 adalah File Sytem di Linux.

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/648x463xgparted-showing-primary-extended-and-logical-partitions.png.pagespeed.ic.foZUIWymSb.png?raw=true)

- APFS, HFS dan HFS+ adalah File System dari MacOS

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/xformat-drive.png.pagespeed.gp+jp+jw+pj+ws+js+rj+rp+rw+ri+cp+md.ic.GnI_H55kwh.png?raw=true)

  

**2. File System Flash**

  
_File system flash_ adalah _file system_ yang didesain untuk menyimpan data pada media _flash memory_. Hal ini menjadi lazim ketika jumlah perangkat mobile semakin banyak dan kapasitas _memory flash_ yang semakin besar. Contohnya pada linux flash filesystems yaitu JFFS, JFFS2, YAFFS, UBIFS, LogFS, F2FS.
  

**3. File System Database**

Konsep baru untuk manajemen _file_ adalah konsep _file system_ berbasis _database_. Sebagai perbaikan bagi Manajemen terstruktur hirarkis, file diidentifikasi oleh karakteristiknya, seperti tipe _file_, topik, pembuat, atau metadata yang sama.

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/GUID-5E2B22F9-F87A-4A95-BD1B-AAC76C9F8462-default.gif?raw=true)

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/kdbfs.png?raw=true)


**4. File System Transaksional**

Beberapa program terkadang membutuhkan perubahan pada beberapa file. Jika pada proses perubahan tadi mengalami kegagalan, maka file akan kembali seperti semula (tidak ada perubahan). Contohnya adalah saat menginstall sebuah software, dimana menjalankan proses writing beberapa file, jika terjadi error selama proses writing, dan software tersebut dibiarkan menjadi **setengah terinstall**, maka software tersebut akan rusak atau tidak stabil. 

Pada File System Transaksional, tidak akan membiarkan hal tersebut terjadi. File System ini menjamin bahwa jika ada suatu proses yang error, maka proses tersebut akan dibatalkan, dan file-file yang telah terbentuk selama proses tadi akan di roll back seperti semula. Contoh dari File System ini pada UNIX adalah Valor File System, Amino, LFS dan TFFS, 
  

**5. File System Jaringan**

  

_File system_ jaringan adalah _file system_ yang bertindak sebagai klien untuk protokol akses file jarak jauh, memberikan akses ke _file_ pada sebuah _server_. Contoh dari _file system_ jaringan ini adalah klien protokol NFS, AFS, SMB, dan klien FTP dan WebDAV.


**6. File System Journaling**

_File system_ yang mencatat setiap perubahan yang terjadi pada storage device ke dalam jurnal (biasanya berupa log sirkular dalam area tertentu) sebelum melakukan perubahan ke _file system_. File sistem seperti ini memiliki kemungkinan yang lebih kecil mengalami kerusakan saat terjadi _power failure_ atau _system crash_.

 
### 1.2 Virtual File System

Virtual file system (VFS) adalah suatu lapisan perangkat lunak dalam kernel yang menyediakan _interface file system_ untuk program _user space_. _Virtual file system_ berfungsi agar berbagai jenis _file system_ dapat diakses oleh aplikasi komputer dengan cara yang seragam. VFS menyediakan antarmuka antara _system call_ dengan sistem yang sesungguhnya.

  

### 1.3 Dentry

Dentry atau **Directory Entry** merupakan sebuah struktur data yang memiliki tugas sebagai penerjemah nama berkas ke inode-nya. Contoh informasi yang disimpan dalam dentry adalah _name_, _pointer to inode_, _pointer to parent dentry_, _use count_, dan lainnya. Adapula command dalam VFS dentry adalah D_compare, D_delete, D_release.

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/figure5.gif?raw=true)

  

### 1.4 Superblock

Setiap _file system_ yang di-_mount_ akan direpresentasikan oleh sebuah VFS Superblock. _Superblock_ digunakan untuk menyimpan informasi mengenai partisi tersebut. _Superblock_ menyimpan informasi sebagai berikut:

-  _Device_: Merupakan _device identifier_, contohnya **/dev/hda1** adalah _harddisk_ pertama yang terdapat pada sistem memiliki device identifier **0×300**.

-  _Inode Pointer_: Merupakan suatu _pointer_ yang menunjuk ke inode pertama pada sistem berkas.

-  _Blocksize_: Menunjukkan ukuran suatu _block_ dari sistem berkas, contohnya **1024** _**bytes**_.

-  _Superblock Operation_: Merupakan suatu pointer ke sekumpulan _superblock routine_ (fungsi) dari _file system_, contohnya _**read**_, _**write**_, dan sebagainya.

-  _File System Type_: Menunjukkan tipe dari _file system_, contoh: EXT2, FAT, NTFS.

-  _File System Specific_: Merupakan suatu pointer ke informasi yang dibutuhkan oleh _file system.

  ![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/figure3.gif?raw=true)



### 1.5 Inode

Inode adalah abstraksi VFS untuk berkas. Setiap berkas, directory, dan data lainnya pada VFS direpresentasikan oleh satu dan hanya satu VFS inode. VFS inode hanya terdapat di memori kernel dan disimpan di inode chace selama masih dibutuhkan oleh sistem. Informasi yang disimpan oleh VFS Inode diantaranya:

-  _Device_: Menunjukan device identifier dari suatu device yang menyimpan berkas ataupun _directory_.

-  _Inode Number_: Merupakan nomor inode yang unik dalam _file system_.

- Mode: Menggambarkan apa yang direpresentasikan oleh VFS inode.

-  _User_ ID: Merupakan identifier bagi pemilik berkas.

-  _Time_: Menunjukkan kapan pembuatan, modifikasi, dan penulisan suatu berkas.

-  _Blocksize_: Menunjukkan ukuran dari _block_ yang digunakan oleh berkas.

-  _Inode Operations_: Merupakan _pointer_ ke suatu _routine_ yang melakukan berbagai operasi pada inode.

-  _Count_: Menunjukkan berapa kali suatu sistem telah menggunakan suatu inode.

-  _Lock_: Digunakan untuk mengunci VFS inode.

-  _File System Specific Information_: Menunjukkan informasi khusus yang dibutuhkan oleh suatu inode.

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/figure4.gif?raw=true)

Berikut adalah hubungan antara dentry, superblock, dan inode pada Virtual File System.

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/daHCZ.gif?raw=true)

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/dentry.JPG?raw=true)

  
# 2. File System in Userspace (FUSE)

FUSE (Filesystem in Userspace) adalah sebuah _interface_ dimana kita dapat membuat _file system_ sendiri pada _userspace_ pada linux.

  

Keuntungan menggunakan FUSE ialah kita dapat menggunakan _library_ apapun yang tersedia untuk membuat _file system_ sendiri tanpa perlu mengenali secara mendalam apa yang _file system_ sebenarnya lakukan di _kernel space_. Hal ini dilakukan karena modul FUSE yang dapat menjembatani antara kode _file system_ yang berada pada _userspace_ dengan _file system_ yang berada pada _kernel space_. Beberapa manfaat yang lain dari FUSE adalah sebagai berikut:

- Dapat dimuat dan dipasang oleh pengguna biasa. Untuk akses jaringan, untuk mendapatkan file arsip, untuk removable media, dll.
- Jika driver sistem FUSE mengalami crash, tidak akan mempengaruhi kernel.
- FUSE dapat dideploy dengan cepat, baik karena tidak perlu intervensi administrator untuk menginstalnya dan karena dapat dengan mudah diakses oleh OS yang didukung.
- Tidak ada masalah lisensi terkait dengan hubungan statis dengan kernel.



![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/fuse.png?raw=true)


#

Salah satu contoh yang menarik dari FUSE adalah [GDFS][7bb7b7cc] (Google Drive File System), dimana GDFS ini memungkinkan kita untuk me-_mount Google Drive_ kita ke sistem linux dan menggunakannya seperti file linux biasa.

  

[7bb7b7cc]: https://github.com/robin-thomas/GDFS  "GDFS"

![enter image description here](https://github.com/Armunz/sisop-modul-4/blob/master/img/google-drive-2.png?raw=true)



Untuk mengimplementasikan FUSE ini, kita harus membuat sebuah program yang terhubung dengan *library*  ```libfuse```. Tujuan dari program yang dibuat ini adalah menspesifikkan bagaimana *file system* merespon *read/write/stat* dari sebuah *request* dan untuk me-*(mount)*  *file system* asli *(kernel space)* ke *file system* yang baru *(userspace)*. Jadi di saat *user* berurusan dengan *read/write/stat request* di *file system (userspace)*, kernel akan meneruskan *input output request* tersebut ke program FUSE dan program tersebut akan merespon kembali ke *user*.

  

Untuk lebih jelasnya mari kita coba membuat program FUSE.

  

### 2.1 Instalasi FUSE

Pertama-tama kita harus memstikan bahwa FUSE sudah ter-install di perangkat anda

```

$ sudo apt update

$ sudo apt install libfuse*

```

  

### 2.2 Cara Kerja FUSE

  

-  ```fuse_main()``` (lib/helper.c) = sebagai fungsi main (userspace), program user memanggil fungsi fuse_main() kemudian fungsi fuse_mount() dipanggil.

-  ```fuse_mount()``` (lib/mount.c) = menciptakan UNIX domain socket, kemudian di fork dan menciptakan child process yang menjalankan fusermount

-  ```fusermount()``` (util/fusermount.c) = untuk mengecek apakah modul FUSE sudah di load. Kemudian membuka /dev/fuse dan mengirim file handle melalu UNIX domain socket kembali ke fungsi fuse_mount()

-  ```fuse_new()``` (lib/fuse.c) = menciptakan struktur data yang berisi ruang yang digukanan untuk menyimpan data file system

-  ```fuse_loop()``` (lib/fuse.c) = membaca file system calls dari /dev/fuse

  

Ini adalah beberapa fungsi yang disediakan oleh **FUSE**:

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

  

### 2.3 Membuat Program FUSE

Fuse memiliki ```struct``` yang dinamakan ```fuse_operations``` yang didefinisikan seperti dibawah ini:

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

#endif

};

```

Semua atribut pada ```struct``` tersebut adalah _pointer_ yang menuju ke fungsi. Setiap fungsi tersebut disebut FUSE saat suatu kejadian yang spesifik terjadi di *file system*. Sebagai contoh saat _user_ menulis di sebuah file, sebuah fungsi yang ditunjuk oleh atribut "write" di ```struct``` akan terpanggil.

  

Selain itu, atribut pada ```struct``` tersebut tertulis seperti fungsi yang biasa digunakan di linux. Contohnya ialah saat kita membuat _directory_ di FUSE maka fungsi mkdir akan dipanggil.

  

**Untuk mengimplementasikan FUSE**, kita harus menggunakan ```struct``` ini dan harus mendefinisikan fungsi yang ada di dalam ```struct``` tersebut. Setelahnya, kita mengisi ```struct``` tersebut dengan pointer dari fungsi yang ingin diimplementasikan.

  

Kebanyakan fungsi-fungsi yang tersedia adalah **opsional**, kita tidak perlu mengimplementasikan semuanya. Beberapa fungsi memang harus diimplementasikan dalam _file system_. Fungsi-fungsi tersebut antara lain:

- Fungsi ```getattr``` yang dipanggil saat sistem mencoba untuk mendapatkan atribut dari sebuah file.

- Fungsi ```readdir``` yang dipanggil saat user mencoba untuk menampilkan file dan direktori yang berada pada suatu direktori yang spesifik.

- Fungsi ```read``` yang dipanggil saat sistem mencoba untuk membaca potongan demi potongan data dari suatu file.

Untuk melihat fungsi-fungsi yang tersedia pada **FUSE** yang lain, buka link berikut: [https://libfuse.github.io/doxygen/structfuse__operations.html](https://libfuse.github.io/doxygen/structfuse__operations.html)

#

Contoh program FUSE sederhana yang hanya menggunakan 3 fungsi tersebut.

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

if (res == -1)

return -errno;

  

return 0;

}

  

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,

off_t offset, struct fuse_file_info *fi)

{

DIR *dp;

struct dirent *de;

  

(void) offset;

(void) fi;

  

dp = opendir(path);

if (dp == NULL)

return -errno;

  

while ((de = readdir(dp)) != NULL) {

struct stat st;

memset(&st, 0, sizeof(st));

st.st_ino = de->d_ino;

st.st_mode = de->d_type << 12;

if (filler(buf, de->d_name, &st, 0))

break;

}

  

closedir(dp);

return 0;

}

  

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,

struct fuse_file_info *fi)

{

int fd;

int res;

  

(void) fi;

fd = open(path, O_RDONLY);

if (fd == -1)

return -errno;

  

res = pread(fd, buf, size, offset);

if (res == -1)

res = -errno;

  

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

  

Setelah itu kode dapat di-_compile_ dengan cara

  

```

gcc -Wall `pkg-config fuse --cflags` [file.c] -o [output] `pkg-config fuse --libs`

```

  

Lalu buat sebuah direktori sebagai tujuan pembuatan FUSE dan menjalankan FUSE pada direktori tersebut.

```

$ mkdir [direktori tujuan]

$ ./[output] [direktori tujuan]

```

Setelah program dijalankan, masuklah kedalam direktori tujuan tersebut. Isi dari direktori tersebut adalah list folder yang sama seperti yang ada di dalam ```root``` atau ```/```.

  

Sesuai dengan penjelasan di awal di mana FUSE dapat memodifikasi _file system_ di _userspace_ tanpa perlu mengubah kode yang ada pada kernel, di sini kita coba memodifikasi kode FUSE tadi agar FUSE tersebut menampilkan apa yang ada di dalam folder ```/home/[user]/Documents```.

Ubah kode FUSE tadi seperti yang ada dibawah ini:

  

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

  

if (res == -1)

return -errno;

  

return 0;

}

  

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,

off_t offset, struct fuse_file_info *fi)

{

char fpath[1000];

if(strcmp(path,"/") == 0)

{

path=dirpath;

sprintf(fpath,"%s",path);

}

else sprintf(fpath, "%s%s",dirpath,path);

int res = 0;

  

DIR *dp;

struct dirent *de;

  

(void) offset;

(void) fi;

  

dp = opendir(fpath);

if (dp == NULL)

return -errno;

  

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

  

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,

struct fuse_file_info *fi)

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

if (fd == -1)

return -errno;

  

res = pread(fd, buf, size, offset);

if (res == -1)

res = -errno;

  

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

### 2.4 Unmount FUSE
Unmount command digunakan untuk "unmount" sebuah filesystem yang telah ter-mount, lalu juga menginformasikan ke sistem untuk menyelesaikan semua operasi read dan write yang masih tertunda agar bisa di-detach (dilepaskan) dengan aman.

Untuk melakukan **unmount** FUSE, jalankan command di bawah ini:
```
sudo umount [direktori tujuan]
atau
fusermount -u [direktori tujuan]
```
  

## Soal Latihan

1. Buat sebuah file system yang mengarah ke /home/[user]/Downloads, file sistem tersebut memiliki sifat mirroring, sehingga nama file yang ada pada folder tersebut akan ditampilkan terbalik. Sebagai contoh ```contohfile.txt``` menjadi ```txt.elifhotnoc```. File system ini hanya memiliki akses read.

2. Buat sebuah file system yang mengarah ke /home/[user]/Documents. File system ini memiliki fitur auto backup, dimana ketika file pada folder tersebut dimodifikasi, file tersebut dengan versi sebelum mengalami perubahan akan disimpan dengan nama ```<namafile>.<time:date>.<ekstensi>``` di dalam direktori ```backup```. Jika folder backup belum ada, maka secara otomatis akan membuat direktori ```backup``` sebelum dipindahkan dan file tidak bisa di read-write-execute selama berada di direktori ```backup```.

  
  

### References

1. https://www.cs.hmc.edu/~geoff/classes/hmc.cs135.201109/homework/fuse/fuse_doc.html

2. http://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/

3. https://github.com/asayler/CU-CS3753-PA5

4. http://amazingharry.blogspot.co.id/2015/01/filesystem-in-userspace-fuse-in-linux_5.html

5. [https://docs.oracle.com/database/121/ADLOB/adlob_fs.htm#ADLOB45989](https://docs.oracle.com/database/121/ADLOB/adlob_fs.htm#ADLOB45989)

6. [http://www.fieldses.org/~bfields/kernel/vfs.txt](http://www.fieldses.org/~bfields/kernel/vfs.txt)

7. [https://developer.ibm.com/technologies/linux/tutorials/l-virtual-filesystem-switch/](https://developer.ibm.com/technologies/linux/tutorials/l-virtual-filesystem-switch/)
