# sisop-modul-4

## Objectives
1. Peserta mengetahui _file system_
2. Peserta mengetahui dan mampu mengimplementasikan FUSE

## Daftar Isi
- [1. File System](https://github.com/dnirmalasari18/sisop-modul-4#1-file-system)
  - [1.1 Tipe File System](https://github.com/dnirmalasari18/sisop-modul-4#11-tipe-file-system)
  - [1.2 Virtual File System](https://github.com/dnirmalasari18/sisop-modul-4#12-virtual-file-system)
  - [1.3 Dentry](https://github.com/dnirmalasari18/sisop-modul-4#13-dentry)
  - [1.4 Superblock](https://github.com/dnirmalasari18/sisop-modul-4#14-superblock)
  - [1.5 Inode](https://github.com/dnirmalasari18/sisop-modul-4#15-inode)
- [2. File System in Userspace (FUSE)](https://github.com/dnirmalasari18/sisop-modul-4#2-file-system-in-userspace-fuse)
  - [2.1 Instalasi Fuse](https://github.com/dnirmalasari18/sisop-modul-4#21-instalasi-fuse)
  - [2.2 Membuat File System dengan Fuse](https://github.com/dnirmalasari18/sisop-modul-4#22-membuat-file-system-dengan-fuse)

# 1. File System
_File system_ adalah struktur logika yang digunakan untuk mengendalikan akses data seperti bagaimana dia disimpan maupun diambil. _File system_ sendiri memiliki banyak jenis dengan penggunaan algoritma yang tentu berbeda.

### 1.1 Tipe File System
**1. File System Disk**

_File system disk_ adalah _file system_ yang didesain untuk menyimpan data pada sebuah media penyimpan data. Contohnya: FAT (FAT 12, FAT 16, FAT 320), NTFS, HFS, HFS+, ext2, ext3, ext4, ISO 9660, ODS-5 dan UDF.

**2. File System Flash**

_File system flash_ adalah _file system_ yang didesain untuk menyimpan data pada media _flash memory_. Hal ini menjadi lazim ketika jumlah perangkat mobile semakin banyak dan kapasitas _memory flash_ yang semakin besar.

**3. File System Database**

Konsep baru untuk manajemen _file_ adalah konsep _file system_ berbasis _database_. Sebagai perbaikan bagi Manajemen terstruktur hirarkis, file diidentifikasi oleh karakteristiknya, seperti tipe _file_, topik, pembuat, atau metadata yang sama.

**4. File System Transaksional**

Setiap operasi _disk_ dapat melibatkan perubahan ke sejumlah _file_ dan _struktur disk_ yang berbeda. Dalam
banyak kasus, perubahan ini berhubungan. Hal ini berarti bahwa operasi ini dieksekusi pada waktu yang sama.

**5. File System Jaringan**

_File system_ jaringan adalah _file system_ yang bertindak sebagai klien untuk protokol akses file jarak jauh, memberikan akses ke _file_ pada sebuah _server_. Contoh dari _file system_ jaringan ini adalah klien protokol NFS, AFS, SMB, dan klien FTP dan WebDAV.

**6. File System untuk Tujuan Khusus**

_File system_ untuk tujuan khusus adalah _file system_ yang tidak termasuk _disk file system_ atau _file system_ jaringan. Yang termasuk dalam kategori ini adalah sistem di mana file ditata secara dinamis oleh _software_, ditujukan untuk tujuan tertentu seperti untuk komunikasi antar proses komputer atau _space file_ sementara. Contohnya _file system procfs_ (/proc) yang dipakai oleh beberapa varian Unix.

**7. File System Journaling**

_File system_ yang mencatat perubahan ke dalam jurnal (biasanya berupa log sirkular dalam area tertentu) sebelum melakukan perubahan ke _file system_. File sistem seperti ini memiliki kemungkinan yang lebih kecil mengalami kerusakan saat terjadi _power failure_ atau _system crash_.

### 1.2 Virtual File System
Virtual file system adalah suatu lapisan perangkat lunak dalam kernel yang menyediakan _interface file system_ untuk program _user space_. _Virtual file system_ berfungsi agar berbagai jenis _file system_ dapat diakses oleh aplikasi komputer dengan cara yang seragam. VFS menyediakan antarmuka antara _system call_ dengan sistem yang sesungguhnya.

### 1.3 Dentry
Dentry atau _Directory Entry_ merupakan sebuah struktur data yang memiliki tugas sebagai penerjemah nama berkas ke inode-nya. Contoh informasi yang disimpan dalam dentry adalah _name_, _pointer to inode_, _pointer to parent dentry_, _use count_, dan lainnya. Adapula command dalam VFS dentry adalah D_compare, D_delete, D_release.

### 1.4 Superblock
Setiap _file system_ yang di-_mount_ akan direpresentasikan oleh sebuah VFS Superblock. _Superblock_ digunakan untuk menyimpan informasi mengenai partisi tersebut. _Superblock_ menyimpan informasi sebagai berikut:
- _Device_: Merupakan _device identifier_, contohnya /dev/hda1 adalah _harddisk_ pertama yang terdapat pada sistem memiliki device identifier 0×300.
- _Inode Pointer_: Merupakan suatu _pointer_ yang menunjuk ke inode pertama pada sistem berkas.
- _Blocksize_: Menunjukkan ukuran suatu _block_ dari sistem berkas, contohnya 1024 _bytes_.
- _Superblock Operation_: Merupakan suatu pointer ke sekumpulan _superblock routine_ (fungsi) dari _file system_, contohnya _read_, _write_, dan sebagainya.
- _File System Type_: Menunjukkan tipe dari _file system_, contoh: EXT2, FAT, NTFS.
- _File System Specific_: Merupakan suatu pointer ke informasi yang dibutuhkan oleh _file system.

### 1.5 Inode
Inode adalah abstraksi VFS untuk berkas. Setiap berkas, _directory_, dan data lainnya pada VFS direpresentasikan oleh satu dan hanya satu VFS inode. VFS inode hanya terdapat di memori kernel dan disimpan di inode chace selama masih dibutuhkan oleh sistem. Informasi yang disimpan oleh VFS Inode diantaranya:
- _Device_: Menunjukan device identifier dari suatu device yang menyimpan berkas ataupun _directory_.
- _Inode Number_: Merupakan nomor inode yang unik dalam _file system_.
- Mode: Menggambarkan apa yang direpresentasikan oleh VFS inode.
- _User_ ID: Merupakan identifier bagi pemilik berkas.
- _Time_: Menunjukkan kapan pembuatan, modifikasi, dan penulisan suatu berkas.
- _Blocksize_: Menunjukkan ukuran dari _block_ yang digunakan oleh berkas.
- _Inode Operations_: Merupakan _pointer_ ke suatu _routine_ yang melakukan berbagai operasi pada inode.
- _Count_: Menunjukkan berapa kali suatu sistem telah menggunakan suatu inode.
- _Lock_: Digunakan untuk mengunci VFS inode.
- _File System Specific Information_: Menunjukkan informasi khusus yang dibutuhkan oleh suatu inode.

# 2. File System in Userspace (FUSE)
FUSE (Filesystem in Userspace) adalah sebuah _interface_ dimana kita dapat membuat _file system_ sendiri pada _userspace_ pada linux.

Keuntungan menggunakan FUSE ialah kita dapat menggunakan _library_ apapun yang tersedia untuk membuat _file system_ sendiri tanpa perlu mengenali secara mendalam apa yang _file system_ sebenarnya lakukan di _kernel space_. Hal ini dilakukan karena modul FUSE yang dapat menjembatani antara kode _file system_ yang berada pada _userspace_ dengan _file system_ yang berada pada _kernel space_.

![FUSE](img/fuse.png)
#
Salah satu contoh yang menarik dari FUSE adalah [GDFS][7bb7b7cc] (Google Drive File System), dimana GDFS ini memungkinkan kita untuk me-_mount Google Drive_ kita ke sistem linux dan menggunakannya seperti file linux biasa.

  [7bb7b7cc]: https://github.com/robin-thomas/GDFS "GDFS"

Untuk mengimplementasikan FUSE ini, kita harus membuat sebuah program yang terhubung dengan *library* ```libfuse```. Tujuan dari program yang dibuat ini adalah menspesifikkan bagaimana *file system* merespon *read/write/stat* dari sebuah *request* dan untuk me-*(mount)* *file system* asli *(kernel space)* ke *file system* yang baru *(userspace)*. Jadi di saat *user* berurusan dengan *read/write/stat request* di *file system (userspace)*, kernel akan meneruskan *input output request* tersebut ke program FUSE dan program tersebut akan merespon kembali ke *user*.

Untuk lebih jelasnya mari kita coba membuat program FUSE.

##### Instalasi FUSE
Pertama-tama kita harus memstikan bahwa FUSE sudah ter-install di perangkat anda
```
$ sudo apt update
$ sudo apt install libfuse*
```

##### Cara Kerja FUSE:

- ```fuse_main()``` (lib/helper.c) = sebagain fungsi main (userspace), program user memanggil fungsi fuse_main() kemudian fungsi fuse_mount() dipanggil.
- ```fuse_mount()``` (lib/mount.c) = menciptakan UNIX domain socket, kemudian di fork dan menciptakan child process yang menjalankan fusermount
- ```fusermount()``` (util/fusermount.c) = untuk mengecek apakah modul FUSE sudah di load. Kemudian membuka /dev/fuse dan mengirim file handle melalu UNIX domain socket kembali ke fungsi fuse_mount()
- ```fuse_new()``` (lib/fuse.c) = menciptakan struktur data yang berisi ruang yang digukanan untuk menyimpan data file system
- ```fuse_loop()``` (lib/fuse.c) = membaca file system calls dari /dev/fuse

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

##### Membuat Program FUSE
Fuse memiliki ```struct``` yang dinamakan ```fuse_operations``` yang didefinisikan seperti dibawah ini:
```c
static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.access		= xmp_access,
	.readlink	= xmp_readlink,
	.readdir	= xmp_readdir,
	.mknod		= xmp_mknod,
	.mkdir		= xmp_mkdir,
	.symlink	= xmp_symlink,
	.unlink		= xmp_unlink,
	.rmdir		= xmp_rmdir,
	.rename		= xmp_rename,
	.link		= xmp_link,
	.chmod		= xmp_chmod,
	.chown		= xmp_chown,
	.truncate	= xmp_truncate,
	.utimens	= xmp_utimens,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
	.statfs		= xmp_statfs,
	.create         = xmp_create,
	.release	= xmp_release,
	.fsync		= xmp_fsync,
  	.setxattr	= xmp_setxattr,
	.getxattr	= xmp_getxattr,
	.listxattr	= xmp_listxattr,
	.removexattr	= xmp_removexattr,
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

Contoh program FUSE sederhanaa yang hanya menggunakan 3 fungsi tersebut.
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

static int xmp_getattr(const char *path, struct stat *stbuf)
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
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
};

int main(int argc, char *argv[])
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

static const char *dirpath = "/home/[user]/Documents";

static int xmp_getattr(const char *path, struct stat *stbuf)
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
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}

```

## Soal Latihan
1. Buat sebuah file sistem yang kamu buat ke /home/[user]/Downloads, lalu ubah nama file yang ada pada folder tersebut menjadi ```[nama file].[ekstensi].bak```. File .bak tersebut hanya dapat dibaca.
2. Buat sebuah file system yang mengarah ke /home/[user]/Documents. Pada saat membuka file dengan ekstensi .pdf, .doc, .txt pada direktori Documents akan muncul pesan error “Terjadi kesalahan! File berisi konten berbahaya.” dan tidak dapat membaca file tersebut. Setelah memunculkan pesan error, file tersebut diganti namanya menjadi ```<namafile>.<ekstensi>.ditandai```. Setelah memunculkan pesan error dan mengganti nama file tadi, file tersebut otomatis dipindahkan ke direktori ```rahasia```. Jika folder rahasia belum ada, maka secara otomatis akan membuat direktori “rahasia” sebelum dipindahkan dan file tidak bisa di read write execute.


### References
1. https://www.cs.hmc.edu/~geoff/classes/hmc.cs135.201109/homework/fuse/fuse_doc.html
2. http://www.maastaar.net/fuse/linux/filesystem/c/2016/05/21/writing-a-simple-filesystem-using-fuse/
3. https://github.com/asayler/CU-CS3753-PA5
4. http://amazingharry.blogspot.co.id/2015/01/filesystem-in-userspace-fuse-in-linux_5.html
