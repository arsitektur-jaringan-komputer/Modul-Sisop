# Modul 3 - Operating System Booting

## Prerequisite

- Menggunakan **VMware Player**, dengan mengaktifkan nested virtualization.
- Install Linux Ubuntu 22.04 dengan minimal 4GB memory dan 4 vCPU.

## Capaian

- Memahami proses booting sistem operasi Linux.
- Memahami konsep dasar kernel Linux dan pembuatan root filesystem (initramfs).
- Mampu mengkompilasi kernel Linux dan membuat root filesystem dengan BusyBox.
- Dapat mengemulasikan sistem operasi dengan QEMU.
- Mampu membuat ISO bootable disk dan menjalankan emulasi menggunakan ISO.

## Daftar Isi

- [i. Prerequisite](#prerequisite)
- [ii. Capaian](#capaian)
- [iii. Daftar Isi](#daftar-isi)
- [1. Pendahuluan](#pendahuluan)
  - [1.1 Pengertian Booting](#pengertian-booting)
- [2. Kernel Linux](#kernel-linux)
  - [2.1 Pengertian Kernel Linux](#kernel-linux)
  - [2.2 Mempersiapkan Kernel Linux](#memperoleh-kernel-linux)
- [3. Membuat Root Filesystem](#membuat-root-filesystem)
  - [3.1 Pengertian Root Filesystem](#pengertian-root-filesystem)
  - [3.2 Single User vs Multi User](#single-user-vs-multi-user)
  - [3.3 Single User](#single-user)
  - [3.4 Multi User](#multi-user)
- [4. Mengemulasikan Sistem Operasi dengan QEMU](#mengemulasikan-sistem-operasi-dengan-qemu)
  - [4.1 Pengertian QEMU](#pengertian-qemu)
  - [4.2 Langkah-Langkah Emulasi dengan QEMU](#langkah-langkah-emulasi-dengan-qemu)
  - [4.3 Pengujian di Dalam Shell](#pengujian-di-dalam-shell)
  - [4.4 Mengulang Proses Booting](#mengulang-proses-booting)
- [5. Membuat ISO Bootable Disk](#membuat-iso-bootable-disk)
- [6. Emulasi Menjalankan File ISO](#emulasi-menjalankan-file-iso)

## Pendahuluan

Bayangkan kamu sedang ingin menyalakan sebuah mobil. Saat kamu memutar kunci atau menekan tombol start, mesin mulai menyala, sistem elektronik aktif, dan dalam beberapa detik mobil siap digunakan. Tapi pernahkah kamu berpikir, apa saja yang sebenarnya terjadi sebelum mobil itu benar-benar siap jalan?

Hal yang sama terjadi saat kamu menyalakan komputer atau laptop. Ketika kamu menekan tombol power, sistem tidak langsung “siap pakai”. Ada proses panjang yang terjadi di belakang layar, inilah yang disebut sebagai proses booting.

### Pengertian Booting

**Booting** adalah proses yang terjadi sejak komputer dinyalakan hingga sistem operasi sepenuhnya aktif dan siap digunakan. Ini adalah tahap awal yang sangat krusial dalam kehidupan sebuah sistem operasi, karena tanpa proses booting yang berhasil, tidak ada aplikasi yang bisa dijalankan.

#### Tahapan Booting secara Umum

Proses booting bisa dibagi menjadi beberapa tahap utama:

1. **Power-On Self Test (POST)**  
   Begitu tombol power ditekan, firmware komputer (seperti BIOS atau UEFI) akan menjalankan POST. Ini adalah proses pengecekan awal untuk memastikan komponen utama seperti RAM, prosesor, dan perangkat penyimpanan berfungsi dengan baik. Jika ada yang bermasalah, proses ini akan menampilkan pesan error atau bunyi beep.

2. **Pencarian Boot Device & Eksekusi Bootloader**  
   Setelah POST selesai, firmware akan mencari **boot device**, yaitu media penyimpanan yang berisi sistem operasi, berdasarkan urutan yang sudah dikonfigurasi (misalnya hard disk, USB, atau CD). Di dalam boot device tersebut, firmware akan mengeksekusi program kecil bernama **bootloader**, seperti GRUB (pada Linux) atau bootmgr (pada Windows). Bootloader bertugas memilih dan memuat kernel sistem operasi ke dalam memori.

3. **Loading Kernel**  
   Kernel adalah inti dari sistem operasi. Ia bertanggung jawab untuk mengatur komunikasi antara perangkat keras dan perangkat lunak, mengelola memori, proses, sistem file, dan lain sebagainya. Setelah dimuat, kernel akan mulai menginisialisasi semua komponen penting sistem.

4. **Menjalankan Init**  
   Setelah kernel selesai melakukan inisialisasi, kontrol sistem akan diberikan ke proses pertama di sistem: **init**. Proses init inilah yang kemudian menjalankan berbagai proses lain, seperti layanan jaringan, login shell, hingga tampilan desktop. Init bisa berupa sistem tradisional seperti SysVinit, atau sistem yang lebih modern seperti systemd.

#### Jenis-Jenis Booting

Terdapat dua jenis proses booting yang umum dikenal:

- **Cold Boot (Hard Boot)**  
  Merupakan proses booting dari keadaan mati total. Misalnya, saat kamu menyalakan laptop yang benar-benar mati, maka proses yang terjadi disebut cold boot.

- **Warm Boot (Soft Boot)**  
  Merupakan proses reboot dari sistem yang sedang menyala. Ini bisa dilakukan, misalnya, dengan menekan tombol restart atau menggunakan perintah `reboot`. Warm boot sering dilakukan setelah melakukan update sistem atau saat ada masalah kecil yang memerlukan restart.

#### Pentingnya Proses Booting dan Peran Kernel

Tanpa proses booting yang benar, sistem operasi **tidak akan pernah bisa dijalankan**. Tidak peduli seberapa canggih atau lengkap perangkat kerasnya, tanpa proses booting, komputer hanyalah tumpukan komponen yang tidak bisa berbuat apa-apa. Proses booting adalah **gerbang awal** yang membuka jalan bagi semua fungsi sistem operasi mulai dari login pengguna, menjalankan aplikasi, hingga mengelola jaringan dan perangkat eksternal.

Setiap tahap dalam proses booting memiliki tanggung jawab penting:

- **Firmware** memastikan bahwa perangkat keras dalam kondisi siap.
- **Bootloader** menentukan sistem operasi mana yang akan dijalankan.
- **Kernel** menginisialisasi semua elemen penting dari sistem.
- **Init** (proses pertama) menjalankan layanan-layanan dasar sistem.

Kegagalan di salah satu tahap ini bisa menyebabkan sistem **gagal boot**, tidak bisa masuk ke sistem operasi, atau bahkan menyebabkan _crash_. Oleh karena itu, memahami bagaimana proses booting bekerja sangat penting, terutama saat mengembangkan sistem operasi atau melakukan troubleshooting.

Salah satu bagian yang paling vital dalam proses ini adalah **kernel**. Kernel bertindak sebagai jembatan antara perangkat keras dan perangkat lunak — ia mengatur komunikasi antara prosesor, memori, perangkat input/output, dan program-program yang berjalan. Dalam sistem berbasis Linux, kernel ini disebut sebagai **Kernel Linux**.

Karena perannya yang sentral, proses booting akan sangat bergantung pada apakah kernel bisa dimuat dan dijalankan dengan baik. Maka dari itu, di bab selanjutnya, kamu akan belajar bagaimana mempersiapkan kernel Linux sebagai fondasi awal dari sistem operasi sederhana yang akan kamu bangun dan emulasikan sendiri.

## Kernel Linux

### Pengertian Kernel Linux

Kernel Linux adalah inti dari sistem operasi Linux. Ia mengatur interaksi antara perangkat keras dan perangkat lunak, serta menangani tugas-tugas penting seperti manajemen memori, proses, sistem file, dan perangkat input/output. Kernel inilah yang pertama kali dijalankan setelah bootloader memuatnya ke dalam memori.

Salah satu keunggulan kernel Linux adalah **sifatnya yang open-source dan modular**. Siapa saja bisa melihat, memodifikasi, dan mengompilasi ulang kernel sesuai kebutuhan. Ini membuatnya ideal untuk keperluan pembelajaran, eksperimen, dan pengembangan sistem operasi yang disesuaikan — seperti yang akan kita lakukan dalam modul ini.

Dalam praktiknya, kita tidak perlu menulis kernel dari nol. Kita cukup mengambil **kode sumber kernel Linux resmi**, lalu menyusunnya (compile) menjadi file biner `bzImage` yang bisa dieksekusi oleh bootloader dan dieksekusi oleh QEMU. Proses penyusunan kernel ini membutuhkan beberapa dependensi sistem serta konfigurasi awal untuk menentukan fitur-fitur yang akan dimasukkan ke dalam kernel hasil build.

### Mempersiapkan Kernel Linux

Pada tahap ini, kita akan mempersiapkan kernel Linux yang akan digunakan untuk membangun sistem operasi minimal menggunakan **QEMU**.

QEMU adalah emulator open-source yang memungkinkan kita untuk menjalankan sistem operasi di dalam lingkungan virtual. Dengan QEMU, kita bisa menguji kernel yang telah kita buat tanpa perlu menginstalnya secara langsung di perangkat keras. Pengertian lebih lanjut terkait QEMU akan dibahas di bagian yang berbeda.

Langkah-langkah yang perlu dilakukan antara lain mengupdate dan menginstall software pendukung, mengunduh kode sumber kernel, melakukan konfigurasi kernel sesuai kebutuhan, serta mengkompilasi kernel untuk menghasilkan file `bzImage`.

1. Update dan Install Software Pendukung

   Sebelum memulai, pastikan sistem telah diperbarui dan semua perangkat lunak yang dibutuhkan untuk proses ini terinstall. Kita akan menggunakan beberapa tools seperti `qemu`, `build-essential`, `bison`, `flex`, dan lainnya.

   ```bash
   sudo apt -y update
   sudo apt -y install qemu-system build-essential bison flex libelf-dev libssl-dev bc grub-common grub-pc libncurses-dev libssl-dev mtools grub-pc-bin xorriso tmux
   ```

2. Siapkan Direktori

   Buat sebuah direktori untuk proyek ini dan pindah ke direktori tersebut. Semua proses pembuatan dan konfigurasi kernel akan dilakukan di dalam direktori ini.

   ```bash
   mkdir -p osboot
   cd osboot
   ```

3. Download dan Ekstrak Kernel Linux

   Langkah berikutnya adalah mengunduh kode sumber kernel Linux versi 6.1.1. Setelah mengunduhnya, kita akan mengekstraknya ke dalam direktori `linux-6.1.1`.

   ```bash
   wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.1.1.tar.xz
   tar -xvf linux-6.1.1.tar.xz
   cd linux-6.1.1
   ```

4. Konfigurasi Kernel

   Kernel Linux perlu dikonfigurasi sebelum dikompilasi. Kita akan memulai dengan menggunakan konfigurasi minimal menggunakan `make tinyconfig`. Setelah itu, kita bisa menyesuaikan konfigurasi lebih lanjut dengan menggunakan `make menuconfig` untuk mengaktifkan fitur-fitur yang diperlukan oleh sistem operasi yang akan kita buat. Beberapa pengaturan yang penting untuk diaktifkan adalah dukungan untuk virtualisasi, file systems, driver perangkat, serta jaringan.

   ```
   64-Bit Kernel
   General Setup > Configure standard kernel features > Enable support for printk
   General Setup > Configure standard kernel features > Enable futex support
   General Setup > Initial RAM filesystem and RAM disk (initramfs/initrd) support
   General Setup > Control Group Support
   Enable the block layer > Legacy autoloading support
   Enable the block layer > Partition type > Advanced Partition Selection
   Device Drivers > Character devices > Enable TTY
   Device Drivers > Character devices > Virtio console
   Device Drivers > Character devices > /dev/mem virtual device support
   Device Drivers > Network device support > Virtio Network Driver
   Device Drivers > Serial Ata / Paralel ATA
   Device Drivers > Block Devices > Virtio block driver
   Device Drivers > Block Devices > loopback device support
   Device Drivers > Block Devices > RAM block device support
   Device Drivers > Virtio drivers
   Device Drivers > Virtualization Drivers
   Device Drivers > Generic  Driver Options > Maintain a devtmpfs at filesystems
   Device Drivers > Generic Driver Options > Automount devtmpfs at /dev
   Executable file formats > Kernel Support for ELF binaries
   Executable file formats > Kernel Support scripts starting with #!
   File Systems > FUSE support
   File Systems > The extended 3 filesystem
   File Systems > The extended 4 filesystem
   File Systems > Second extended fs support
   File Systems > Virtio Filesystem
   File Systems > Kernel automounter support
   File Systems > Pseudo File Systems > /proc file system support
   File Systems > Pseudo File Systems > sysctl support
   File Systems > Pseudo File Systems > sysfs file system support
   Networking Support > Networking options > Unix domain sockets
   Networking Support > Networking options > TCP/IP Networking
   ```

   Konfigurasi ini akan memastikan bahwa kernel yang dihasilkan dapat bekerja dengan baik di lingkungan virtual dan mendukung operasi dasar yang diperlukan.

   ```bash
   make tinyconfig
   make menuconfig
   ```

   Setelah selesai mengonfigurasi, kamu bisa langsung menyimpan file konfigurasi `.config` dan melanjutkan ke langkah berikutnya.

5. Kompilasi Kernel

   Dengan konfigurasi yang sudah siap, kini kita bisa memulai proses kompilasi kernel. Proses kompilasi ini akan menghasilkan file `bzImage`, yang merupakan file kernel yang siap digunakan untuk booting.

   ```bash
   make -j$(nproc)
   ```

   Perintah di atas akan mengkompilasi kernel menggunakan seluruh inti CPU yang tersedia pada sistem.

6. Hasilkan File `bzImage`

   Setelah kompilasi selesai, kita akan mendapatkan file `bzImage` di dalam direktori `arch/x86/boot/`. Pindahkan file ini ke direktori `osboot` untuk persiapan langkah selanjutnya, yaitu pembuatan root filesystem dan emulasi menggunakan QEMU.

   ```bash
   cp arch/x86/boot/bzImage ..
   ```

## Membuat Root Filesystem

### Pengertian Root Filesystem

Bayangkan sistem operasi seperti sebuah rumah. Maka **root filesystem** adalah _pondasi dan kerangka dasar rumah tersebut_. Semua ruangan (direktori seperti `/bin`, `/etc`, `/dev`, dll) dibangun di atas rootfs ini, dan semua aktivitas penghuni (aplikasi dan layanan) bergantung padanya agar rumah bisa berfungsi dengan baik. Tanpa rootfs, rumah itu seperti ada bangunan tapi tanpa jalan masuk, alat, atau aturan.

#### Apa itu Root Filesystem?

- **Root Filesystem** adalah struktur berkas yang digunakan oleh sistem operasi untuk menyimpan file-file esensial yang diperlukan untuk menjalankan sistem, seperti konfigurasi, perangkat keras, dan aplikasi dasar.
- Ini adalah **filesystem pertama yang dimuat** setelah proses booting dimulai dan menjadi pusat penyimpanan bagi semua file penting yang dibutuhkan oleh sistem.

#### Komponen dalam Root Filesystem

Root filesystem biasanya berisi beberapa direktori penting, antara lain:

- **`/bin`** – Menyimpan aplikasi dan program dasar yang digunakan untuk menjalankan sistem.
- **`/etc`** – Menyimpan file konfigurasi sistem dan aplikasi.
- **`/lib`** – Berisi pustaka sistem yang diperlukan oleh program untuk dijalankan.
- **`/dev`** – Menyimpan file perangkat yang diperlukan untuk mengakses hardware.
- **`/home`** – Direktori tempat menyimpan data pengguna.

#### Initial RAM Filesystem (initramfs)

Sebelum root filesystem utama bisa dipasang, sistem membutuhkan suatu filesystem sementara yang disebut **initramfs** untuk memulai sistem operasi. Initramfs adalah sebuah **image filesystem yang sangat ringan** yang dimuat ke dalam memori pada saat awal booting.

**Fungsi Initramfs**:

- Menyediakan alat dasar dan skrip untuk mempersiapkan sistem.
- Mengonfigurasi perangkat keras dan memeriksa perangkat penyimpanan.
- Menyediakan kernel dengan informasi yang dibutuhkan untuk memasang root filesystem utama.

#### BusyBox

Untuk mempermudah tugas-tugas dalam initramfs, **BusyBox** digunakan sebagai kumpulan alat yang sangat ringan namun lengkap. BusyBox menyediakan berbagai **utilitas sistem** yang digunakan untuk mengonfigurasi perangkat keras, memanipulasi sistem berkas, dan melakukan tugas penting lainnya selama proses booting.

**Fungsi BusyBox**:

- Mengganti banyak perintah standar Linux yang ada di sistem operasi dengan versi yang lebih ringan dan sederhana.
- Berfungsi dalam lingkungan terbatas sumber daya (seperti sistem embedded atau tahap awal booting).
- Menyediakan utilitas seperti `ls`, `cp`, `mv`, `mount`, dan banyak lainnya, yang digunakan oleh initramfs untuk melakukan berbagai tugas.

**Instalasi BusyBox**

BusyBox dapat diinstal dengan menggunakan perintah berikut:

```bash
sudo apt install -y busybox-static
```

Kamu bisa mengecek apakah BusyBox sudah terinstal dengan perintah:

```bash
whereis busybox
```

Biasanya, **BusyBox** akan ditemukan di direktori `/usr/bin/busybox`.

### Single User vs Multi User

Sistem operasi Linux dapat dijalankan dalam dua mode utama berdasarkan kebutuhan pengguna dan kompleksitas sistem: **Single User** dan **Multi User**. Keduanya menggunakan root file system sebagai pondasi sistem, namun memiliki pendekatan, struktur, dan proses booting yang berbeda.

Mode **Single User** cocok untuk keperluan pemeliharaan sistem atau debugging karena hanya memperbolehkan satu pengguna (biasanya root) untuk mengakses sistem. Sementara itu, mode **Multi User** dirancang agar beberapa pengguna bisa login dan bekerja secara bersamaan, dengan pengelolaan hak akses, akun, dan layanan sistem yang lebih kompleks.

#### Perbandingan: Single User vs Multi User

1. Dari Sisi Fungsi

   | Aspek              | Single User                                                                   | Multi User                                                                            |
   | ------------------ | ----------------------------------------------------------------------------- | ------------------------------------------------------------------------------------- |
   | **Tujuan**         | Digunakan untuk pemeliharaan, debugging, atau sistem minimal.                 | Digunakan untuk penggunaan sehari-hari, server, atau sistem dengan banyak pengguna.   |
   | **Akses Pengguna** | Hanya satu pengguna (biasanya root) yang dapat login.                         | Banyak pengguna bisa login secara bersamaan dengan hak akses berbeda.                 |
   | **Interaksi**      | Tidak ada interaksi antar pengguna, fokus pada tugas sistem dasar.            | Mendukung interaksi antar pengguna dan proses secara paralel.                         |
   | **Keamanan**       | Minim pengaturan keamanan karena hanya root yang akses.                       | Perlu sistem autentikasi, manajemen user, dan hak akses untuk masing-masing pengguna. |
   | **Kegunaan**       | Praktis untuk recovery, tes booting, atau sistem embedded yang sangat ringan. | Umum pada sistem desktop, server, atau lingkungan kerja yang melibatkan banyak user.  |

2. Dari Sisi Pembuatan Sistem

   **Single User**

   - **Struktur Root File System:**
     - Minimal: `/bin`, `/dev`, `/proc`, `/sys`, dan file `init`.
     - Tidak ada direktori pengguna (`/home`, `/etc/passwd`, dll).
   - **File `init`:**
     - Sederhana. Biasanya hanya berisi:
       ```sh
       #!/bin/sh
       echo "Init started"
       exec /bin/sh
       ```
     - Setelah mounting, langsung masuk ke shell sebagai root tanpa proses login.
   - **Manajemen User:** Tidak diperlukan, karena hanya root yang menjalankan shell.
   - **Proses Booting:** Cepat dan sederhana, tidak ada daemon login atau manajemen hak akses.

   **Multi User**

   - **Struktur Root File System:**
     - Lebih lengkap: Menyertakan `/etc/passwd`, `/etc/group`, `/etc/shadow`, `/home/user1`, dll.
     - Menggunakan BusyBox untuk `login`, `adduser`, `getty`, dan tool manajemen sistem lainnya.
   - **File `init`:**
     - Lebih kompleks, karena harus menjalankan proses login melalui `getty`:
       ```sh
       #!/bin/sh
       mount -t proc none /proc
       mount -t sysfs none /sys
       mount -t devtmpfs none /dev
       /bin/hostname multiuser
       setsid /bin/getty -n -l /bin/login 115200 tty1 &
       exec /bin/sh
       ```
     - Proses login aktif di `tty1`, dan bisa ditambah untuk `tty2`, `tty3`, dst.
   - **Manajemen User:**
     - Harus membuat user dan grup menggunakan `adduser` atau manual edit `/etc/passwd`.
     - Password bisa dienkripsi atau dibuat kosong tergantung konfigurasi BusyBox.
   - **Proses Booting:** Sistem menjalankan login prompt pada beberapa terminal (multi-console). Setelah login, setiap user mendapatkan shell-nya masing-masing sesuai hak akses.

3. Ringkasan Perbedaan

   | Aspek                        | Single User                                  | Multi User                                            |
   | ---------------------------- | -------------------------------------------- | ----------------------------------------------------- |
   | **Tujuan Utama**             | Pemeliharaan / sistem minimal                | Sistem siap pakai oleh banyak user                    |
   | **Akses**                    | Hanya root                                   | Banyak user dengan login                              |
   | **Kompleksitas File System** | Sederhana                                    | Lengkap dengan konfigurasi user                       |
   | **File Init**                | Shell langsung                               | Proses login via getty                                |
   | **Direktori Tambahan**       | Tidak diperlukan `/etc/passwd`, `/home`, dll | Wajib menyertakan direktori dan file konfigurasi user |
   | **Manajemen User**           | Tidak ada                                    | Perlu sistem autentikasi                              |
   | **Contoh Kasus**             | Rescue mode, sistem embedded                 | Server, multi-user workstation                        |

Kesimpulan

Perbedaan utama antara **single user** dan **multi user** tidak hanya terletak pada jumlah pengguna yang bisa login, tapi juga pada tingkat kompleksitas sistem yang dibangun. Mode **single user** menawarkan kesederhanaan dan kecepatan, ideal untuk kondisi darurat atau sistem embedded yang ringan. Sementara itu, mode **multi user** membutuhkan konfigurasi tambahan, namun memberikan fleksibilitas dan skalabilitas lebih besar yang cocok untuk sistem produksi.

### Single User

Setelah mempersiapkan kernel dan root filesystem, kita akan melanjutkan ke pembuatan sistem file yang lebih sederhana yang berfungsi untuk tahap awal booting, yaitu Single User Mode. Pada mode **Single User**, root filesystem yang telah disiapkan akan dijalankan dengan hanya satu pengguna yang memiliki kontrol penuh terhadap sistem. Mode ini berguna pada tahap awal booting atau untuk sistem yang lebih sederhana, seperti embedded systems, di mana hanya satu pengguna yang diperlukan untuk mengonfigurasi atau mengelola sistem. Berikut adalah penjelasan lebih lanjut tentang bagaimana root filesystem bekerja dalam mode single user, beserta langkah-langkah yang dapat diikuti untuk membuat root filesystem ini.

#### Konsep Root Filesystem dalam Mode Single User

1. **Peran Program `init`**

   - Setelah kernel Linux memulai proses booting, langkah pertama yang dilakukan adalah menjalankan program **init**. Program **init** ini adalah program pertama yang dijalankan oleh kernel dengan Process ID (PID) 1.
   - **init** bertanggung jawab untuk menyiapkan lingkungan yang dibutuhkan oleh sistem dan memulai proses lainnya dalam urutan yang tepat.
   - Root filesystem yang dimuat pertama kali oleh kernel akan menyertakan program **init**, yang biasanya terletak di dalam direktori root (`/`).

2. **Penggunaan Initramfs dalam Single User**

   - Untuk mempermudah proses booting, root filesystem dapat dimuat menggunakan **initramfs**. Initramfs adalah sebuah **filesystem sementara** yang dimuat ke dalam memori dan berfungsi untuk mempersiapkan sistem sebelum root filesystem utama dipasang.
   - Dalam kasus ini, **initramfs** digunakan untuk mengonfigurasi sistem dan menyediakan alat minimal yang dibutuhkan, seperti BusyBox, untuk menjalankan program **init** dalam mode single user.

3. **Mengapa Menggunakan BusyBox**

   - Pada sistem operasi yang lebih kompleks, root filesystem biasanya disimpan pada penyimpanan permanen seperti hard drive atau SSD. Namun, dalam proses booting awal atau pada sistem dengan sumber daya terbatas, kita menggunakan **BusyBox**.
   - **BusyBox** adalah kumpulan utilitas Unix/Linux yang dikemas dalam satu file eksekusi. Ini memungkinkan sistem yang lebih ringan dengan menyediakan berbagai perintah penting seperti `ls`, `cp`, `mount`, dan `sh`, yang digunakan untuk mengonfigurasi dan mengelola filesystem.

#### Langkah-Langkah Pembuatan Root File System untuk Single User Mode

Berikut adalah langkah-langkah pembuatan root filesystem yang menggunakan initramfs dan BusyBox untuk menyediakan lingkungan sistem yang minimal namun fungsional.

1. **Masuk ke Mode Superuser**

   Untuk membuat perubahan pada sistem, kita akan bekerja dalam mode superuser (root). Gunakan perintah berikut untuk masuk ke shell sebagai root:

   ```bash
   sudo bash
   ```

2. **Buat Direktori untuk Initramfs**

   Selanjutnya, kita buat direktori `myramdisk` dan beberapa sub-direktori untuk sistem file:

   ```bash
   mkdir -p myramdisk/{bin,dev,proc,sys}
   ```

   Struktur direktori ini akan terlihat seperti ini:

   ```
   myramdisk/
     ├── bin/
     ├── dev/
     ├── proc/
     └── sys/
   ```

3. **Salin File Device ke Direktori `dev`**

   Direktori `/dev` di Linux berisi file perangkat penting seperti `null`, `tty`, `zero`, dan `console`. Kita akan menyalinnya dari sistem host ke dalam direktori `dev` di `myramdisk`.

   ```bash
   cp -a /dev/null myramdisk/dev
   cp -a /dev/tty* myramdisk/dev
   cp -a /dev/zero myramdisk/dev
   cp -a /dev/console myramdisk/dev
   ```

4. **Salin File BusyBox ke Direktori `bin`**

   Selanjutnya, kita akan menyalin file **BusyBox** ke direktori `bin` yang telah dibuat di dalam `myramdisk`. Kemudian, kita akan menginstal semua utilitas yang disediakan oleh BusyBox:

   ```bash
   cp /usr/bin/busybox myramdisk/bin
   cd myramdisk/bin
   ./busybox --install .
   ```

   Perintah ini akan menambahkan berbagai perintah Unix/Linux seperti `ls`, `cp`, `mv`, `mount`, dan banyak lainnya ke dalam direktori `bin`.

5. **Buat File `init`**

   Di dalam direktori `myramdisk`, buat file **init** yang akan dijalankan pertama kali saat booting. Isi file ini dengan instruksi untuk melakukan mounting sistem file dan memulai shell interaktif.

   ```bash
   #!/bin/sh
   /bin/mount -t proc none /proc
   /bin/mount -t sysfs none /sys
   exec /bin/sh
   ```

   File ini akan melakukan mounting untuk `proc` dan `sysfs`, yang memungkinkan sistem berkomunikasi dengan kernel. Setelah itu, akan memulai shell `/bin/sh` untuk interaksi pengguna.

6. **Berikan Izin Eksekusi pada File `init`**

   Agar file `init` dapat dieksekusi, kita perlu memberikan izin eksekusi dengan perintah:

   ```bash
   chmod +x init
   ```

7. **Kompresi dan Buat File `initramfs`**

   Setelah menyiapkan semua file yang diperlukan, langkah berikutnya adalah mengumpulkan file-file tersebut menggunakan perintah `cpio` dan mengompresnya dengan `gzip` untuk membuat image initramfs.

   ```bash
   find . | cpio -oHnewc | gzip > ../myramdisk.gz
   ```

   Perintah ini akan menghasilkan file **myramdisk.gz**, yang berisi sistem file minimal untuk tahap awal booting.

Dengan langkah-langkah di atas, kita telah berhasil membuat root filesystem dalam mode single user dengan menggunakan BusyBox dan initramfs.

### Multi User

Setelah sistem berhasil dijalankan dalam mode **single user**, tahap selanjutnya adalah membangun sistem dalam mode **multi user** menggunakan **initramfs**. Pada mode ini, lebih dari satu pengguna dapat login dan menggunakan sistem secara bersamaan, sehingga cocok untuk penggunaan umum yang membutuhkan manajemen akun dan akses.

Untuk mendukung multi user, kita akan menyusun root file system yang lebih lengkap. Kita menggunakan **BusyBox** sebagai penyedia utilitas dasar Linux, dan **initramfs** sebagai sistem file root yang dimuat ke memori saat booting. Berbeda dengan single user yang hanya menyediakan shell langsung untuk root, mode ini memerlukan proses login di terminal, sehingga dibutuhkan file seperti `/etc/passwd`, `/etc/group`, dan layanan seperti `getty`.

#### Langkah-Langkah Pembuatan Root File System dengan Multi User

1. **Masuk ke Mode Privileged (Superuser)**

   Agar bisa membuat perubahan pada sistem, kita akan bekerja dalam mode **superuser** (root) dengan perintah:

   ```bash
   sudo bash
   ```

2. **Buat Direktori untuk Initramfs**

   Buat direktori dengan nama `myramdisk` dan beberapa sub-direktori yang diperlukan oleh sistem file root.

   ```bash
   mkdir -p myramdisk/{bin,dev,proc,sys,etc,root,home/user1}
   ```

3. **Salin File Device ke Direktori `dev`**

   Direktori `/dev` berisi perangkat penting seperti `null`, `tty`, `zero`, dan `console`. Kita akan menyalin file-file perangkat ini dari sistem host ke dalam direktori `dev` di `myramdisk`.

   ```bash
   cp -a /dev/null myramdisk/dev
   cp -a /dev/tty* myramdisk/dev
   cp -a /dev/zero myramdisk/dev
   cp -a /dev/console myramdisk/dev
   ```

4. **Salin BusyBox ke Direktori `bin`**

   Salin file **BusyBox** ke direktori `bin` dan jalankan instalasi utilitas BusyBox:

   ```bash
   cp /usr/bin/busybox myramdisk/bin
   cd myramdisk/bin
   ./busybox --install .
   ```

5. **Buat Password Root**

   Gunakan perintah `openssl` untuk membuat password root yang dienkripsi:

   ```bash
   openssl passwd -1 myrootpassword
   ```

   Salin hasil yang keluar dari perintah di atas, karena ini akan digunakan untuk membuat database user dan password.

6. **Buat File `passwd` di Direktori `etc`**

   Buat file `passwd` di direktori `etc` untuk menyimpan informasi tentang akun root dan user yang akan kita buat (misalnya `user1`):

   ```bash
   root:<<hasilgeneratorrootpassword>>:0:0:root:/root:/bin/sh
   user1:<<hasilgeneratorrootpassword>>:1001:100:user1:/home/user1:/bin/sh
   ```

7. **Buat File `group` di Direktori `etc`**

   Buat file `group` di direktori `etc` yang berisi pengaturan grup untuk user yang baru dibuat:

   ```bash
   root:x:0:
   bin:x:1:root
   sys:x:2:root
   tty:x:5:root,user1
   disk:x:6:root
   wheel:x:10:root,user1
   users:x:100:user1
   ```

8. **Buat File `init`**

   Kembali ke direktori `myramdisk`, buat file **init** yang akan dipanggil oleh bootloader saat sistem booting. Isi file `init` dengan instruksi untuk melakukan mounting sistem file dan menjalankan proses login:

   ```bash
   #!/bin/sh
   /bin/mount -t proc none /proc
   /bin/mount -t sysfs none /sys

   while true
   do
       /bin/getty -L tty1 115200 vt100
       sleep 1
   done
   ```

   File ini akan melakukan mounting untuk `proc` dan `sysfs`, yang memungkinkan komunikasi antara user space dan kernel. Kemudian, perintah `getty` akan menunggu input login dari pengguna melalui konsol.

9. **Berikan Izin Eksekusi pada File `init`**

   Agar file `init` bisa dieksekusi, kita perlu memberikan izin eksekusi pada file tersebut:

   ```bash
   chmod +x init
   ```

10. **Kompresi dan Buat File `initramfs`**

    Setelah menyiapkan semua file yang diperlukan, kita akan mengumpulkan file-file tersebut menggunakan utilitas **cpio** dan kemudian mengompresnya dengan **gzip** untuk membuat image **initramfs**.

    ```bash
    find . | cpio -oHnewc | gzip > ../myramdisk.gz
    ```

    Perintah ini akan menghasilkan file **myramdisk.gz** yang berisi sistem file minimal untuk booting.

Dengan langkah-langkah di atas, kita telah berhasil membuat root filesystem dengan kemampuan multi-user menggunakan initramfs dan BusyBox. Sistem ini memungkinkan beberapa pengguna untuk login, dengan satu pengguna utama (root) dan satu pengguna tambahan (user1). Ketika sistem boot, pengguna dapat login menggunakan kredensial yang telah kita tentukan.

## Emulasi dengan QEMU

### Pengertian QEMU

**QEMU** (Quick Emulator) adalah program yang digunakan untuk menjalankan **sistem operasi** atau **aplikasi** di dalam **lingkungan virtual** (mirip dengan komputer lain) tanpa memerlukan perangkat keras fisik yang berbeda. Dengan QEMU, kita bisa menjalankan sistem operasi yang berbeda, seperti Linux di atas Windows, atau mencoba sistem yang sama sekali berbeda, misalnya ARM atau RISC-V, di atas mesin berbasis x86 (komputer biasa).

Bayangkan kamu ingin mencoba sistem yang berbeda, misalnya kamu ingin menguji **Windows 10** di atas **Mac**. Daripada menginstal Windows di hard drive Mac secara langsung (yang bisa memakan waktu dan berisiko), kamu bisa menggunakan QEMU untuk **"meniru"** mesin Windows di dalam Mac, sehingga kamu bisa menguji Windows 10 tanpa mengganti apa pun di komputer asli.

QEMU bekerja dengan **meniru** (emulasi) komponen-komponen dalam komputer seperti **CPU** (prosesor), **memori**, dan **perangkat keras** lainnya. Dengan begitu, kamu bisa menjalankan sistem operasi atau aplikasi yang biasanya hanya bisa berjalan di mesin tertentu, tanpa memerlukan perangkat keras yang sesuai.

#### Perbedaan antara QEMU dan Virtual Machine (VM)

**QEMU** dan **Virtual Machine (VM)** sering kali digunakan untuk hal yang sama, yaitu menjalankan sistem operasi yang berbeda, tetapi keduanya berbeda dalam cara kerjanya.

1. **Virtual Machine (VM)**

   - **Apa itu VM?** VM adalah **mesin virtual** yang berjalan di atas sistem operasi utama. Dengan VM, kamu membuat **mesin virtual** yang berfungsi seperti komputer fisik terpisah. Kamu bisa menjalankan sistem operasi (misalnya Linux) di atas sistem operasi lain (misalnya Windows) dengan menggunakan program seperti **VirtualBox** atau **VMware**.
   - **Cara Kerja VM**: Mesin virtual menggunakan **virtualisasi** untuk membagi sumber daya komputer fisik (seperti CPU, RAM) ke dalam beberapa mesin virtual. Jadi, meskipun semua mesin virtual berbagi komputer fisik yang sama, mereka bekerja seolah-olah mereka memiliki komputer sendiri.

2. **QEMU**

   - **Apa itu QEMU?** QEMU adalah **emulator** yang lebih fleksibel. QEMU bisa melakukan **emulasi** penuh dari komputer, yang berarti dia bisa meniru **perangkat keras** (seperti CPU dan perangkat lainnya) dari mesin lain. Ini memungkinkan kita untuk menjalankan sistem operasi yang dirancang untuk perangkat keras yang berbeda (misalnya ARM, MIPS) di atas mesin yang berbeda (misalnya x86).
   - **Cara Kerja QEMU**: QEMU tidak hanya **virtualisasi** tetapi juga **emulasi**. Artinya, QEMU bisa "meniru" perangkat keras secara penuh, memungkinkan kita menjalankan berbagai jenis sistem operasi tanpa memerlukan mesin fisik yang sesuai.

3. **Perbandingan QEMU dan VM**

   | **Aspek**             | **QEMU**                                                                          | **VM**                                                                                                 |
   | --------------------- | --------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------ |
   | **Jenis Teknologi**   | Emulasi (meniru perangkat keras)                                                  | Virtualisasi (menggunakan sumber daya bersama)                                                         |
   | **Fungsi Utama**      | Menjalankan sistem operasi dari arsitektur yang berbeda                           | Menjalankan sistem operasi dengan membagi sumber daya komputer                                         |
   | **Contoh Penggunaan** | Menjalankan sistem operasi untuk arsitektur yang berbeda (misal: ARM di atas x86) | Menjalankan berbagai sistem operasi di satu mesin fisik (misal: Windows dan Linux dalam satu komputer) |

### Langkah-Langkah Emulasi dengan QEMU

Setelah membuat file `bzImage` (kernel) dan `myramdisk.gz` (initramfs), kita bisa menjalankan sistem operasi hasil build kita menggunakan **QEMU**. Berikut langkah-langkah yang perlu dilakukan:

1. Pindah ke Direktori `osboot`

   Pertama, masuk ke direktori tempat kamu menyimpan file hasil build:

   ```bash
   cd osboot
   ```

   Direktori ini seharusnya berisi file berikut:

   - `bzImage` → kernel hasil kompilasi
   - `myramdisk.gz` → root filesystem yang telah dibuat

2. Jalankan Emulasi dengan QEMU

   Gunakan perintah berikut untuk menjalankan QEMU dengan konfigurasi tertentu:

   ```bash
   qemu-system-x86_64 \
   -smp 2 \
   -m 256 \
   -display curses \
   -vga std \
   -kernel bzImage \
   -initrd myramdisk.gz
   ```

   Penjelasan Opsi:

   | Opsi                   | Fungsi                                                                |
   | ---------------------- | --------------------------------------------------------------------- |
   | `-smp 2`               | Menyediakan 2 vCPU (virtual CPU) untuk sistem yang diemulasi          |
   | `-m 256`               | Memberikan alokasi memori sebesar 256 MB                              |
   | `-display curses`      | Menampilkan output dalam mode teks (bisa digunakan di terminal biasa) |
   | `-vga std`             | Mengatur tampilan VGA standar (80x25), cukup untuk tampilan teks      |
   | `-kernel bzImage`      | Menentukan kernel yang akan digunakan untuk booting                   |
   | `-initrd myramdisk.gz` | Menentukan root filesystem (initramfs) yang akan dimuat               |

Setelah menjalankan perintah di atas, QEMU akan mulai melakukan proses booting. Jika tidak ada kesalahan, kamu akan masuk ke shell BusyBox (jika menggunakan `init` yang diarahkan ke shell), atau melihat proses login jika menggunakan konfigurasi multi-user.

### Pengujian di Dalam Shell

Setelah sistem berhasil boot melalui QEMU, kamu akan melihat tampilan shell atau prompt login—tergantung apakah kamu menggunakan mode **single user** atau **multi user**.

#### **Single User**

Kamu akan langsung masuk ke shell `sh` yang disediakan oleh **BusyBox**. Ini artinya sistem hanya menyediakan akses langsung ke terminal tanpa proses login.

Contoh tampilan:

```
/ #
```

Di sini kamu bisa mencoba beberapa perintah dasar Linux seperti:

```sh
ls
cat /proc/cpuinfo
echo "Hello dari QEMU!"
```

Tujuannya adalah memastikan bahwa:

- File system berhasil dimuat
- BusyBox berjalan dengan baik
- Sistem responsif terhadap input

#### **Multi User**

Pada mode ini, kamu akan melihat prompt login. Sistem akan meminta kamu untuk masuk menggunakan username dan password yang sesuai dengan yang telah kamu konfigurasikan sebelumnya (misalnya di file `/etc/passwd` dan `/etc/shadow`).

Contoh tampilan:

```
Welcome to Mini Linux
myos login: user1
Password:
```

Setelah berhasil login, kamu akan masuk ke shell dan bisa melakukan pengujian seperti:

```sh
whoami
ls /home
```

Hal yang perlu diuji:

- Proses login berjalan dengan benar
- Hak akses pengguna sesuai (tidak bisa akses direktori root tanpa izin)
- Bisa berpindah direktori dan menjalankan perintah dasar

#### Mengulang Proses Booting

Jika kamu ingin **mengulang proses booting** sistem yang telah kamu emulasikan dengan QEMU, kamu perlu menghentikan proses QEMU terlebih dahulu, lalu menjalankannya kembali seperti sebelumnya.

1. **Matikan proses QEMU**

   Gunakan perintah berikut untuk menghentikan proses QEMU:

   ```sh
   pkill -f qemu
   ```

   Perintah ini akan mencari proses yang mengandung kata “qemu” dan menghentikannya. Pastikan kamu tidak sedang menjalankan proses penting lain yang juga mengandung kata tersebut.

2. **Jalankan kembali sistem menggunakan QEMU**

   Ulangi perintah QEMU yang sama seperti sebelumnya:

   ```sh
   qemu-system-x86_64 \
     -smp 2 \
     -m 256 \
     -display curses \
     -vga std \
     -kernel bzImage \
     -initrd myramdisk.gz
   ```

Ini akan mem-boot ulang sistem dari awal dengan pengaturan yang sama, memungkinkan kamu untuk menguji kembali apakah sistem berjalan stabil dan seperti yang diharapkan.

## Membuat ISO Bootable Disk

**ISO bootable disk** adalah sebuah file berformat `.iso` yang berisi salinan struktur sistem file lengkap dari sistem operasi dan dapat digunakan untuk melakukan **booting**. File ini bisa dijalankan lewat emulator seperti QEMU, diburning ke CD/DVD, atau ditulis ke USB untuk menjalankan sistem secara langsung (live system).

Membuat ISO ini berguna untuk:

- **Distribusi sistem operasi** buatan sendiri
- **Pengujian dan deployment cepat** ke mesin lain
- Membuat **live Linux minimal** hasil kustomisasi kita sendiri

Agar file ISO bisa digunakan untuk booting, kita memerlukan **bootloader**, yaitu program awal yang dijalankan saat komputer menyala. Di sini kita akan menggunakan **GRUB** (GRand Unified Bootloader) sebagai bootloader-nya, dan menggabungkannya dengan kernel (`bzImage`) serta root filesystem (`myramdisk.gz`) ke dalam satu ISO menggunakan tool `grub-mkrescue`.

**Langkah-langkah :**

1. **Masuk ke direktori `osboot`**:

   ```bash
   cd osboot
   ```

2. **Buat struktur direktori ISO**:

   ```bash
   mkdir -p mylinuxiso/boot/grub
   ```

3. **Salin file kernel dan root filesystem**:

   ```bash
   cp bzImage mylinuxiso/boot
   cp myramdisk.gz mylinuxiso/boot
   ```

4. **Buat file konfigurasi GRUB**:
   Buat file `grub.cfg` di `mylinuxiso/boot/grub` dengan isi:

   ```cfg
   set timeout=5
   set default=0

   menuentry "MyLinux" {
     linux /boot/bzImage
     initrd /boot/myramdisk.gz
   }
   ```

   > File ini akan membuat menu GRUB yang menampilkan pilihan boot bernama "MyLinux", dan mengarahkan sistem untuk menggunakan kernel dan initrd yang sudah kita sediakan.

5. **Buat file ISO bootable**:
   Jalankan perintah berikut dari direktori `osboot`:
   ```bash
   grub-mkrescue -o mylinux.iso mylinuxiso
   ```

## Emulasi Menjalankan File ISO

Setelah berhasil membuat file **ISO bootable**, langkah selanjutnya adalah **menguji ISO tersebut dengan emulator**. Di sini kita akan menggunakan **QEMU** untuk menjalankan sistem yang telah kita bangun, layaknya booting dari CD-ROM virtual.

Emulasi ini memungkinkan kita untuk:

- Melihat apakah sistem bootable ISO kita berjalan dengan baik
- Menjalankan shell BusyBox dari ISO
- Mengecek hasil integrasi kernel dan initramfs

**Langkah-langkah Emulasi:**

1. **Masuk ke direktori `osboot`**:

   ```bash
   cd osboot
   ```

2. **Jalankan sistem dengan QEMU menggunakan file ISO**:

   ```bash
   qemu-system-x86_64 \
     -smp 2 \
     -m 256 \
     -display curses \
     -vga std \
     -cdrom mylinux.iso
   ```

   **Penjelasan parameter**:

   - `-smp 2` → menggunakan 2 CPU virtual
   - `-m 256` → alokasi RAM sebesar 256 MB
   - `-display curses` → menampilkan output di terminal (text mode)
   - `-vga std` → VGA standard untuk menampilkan resolusi 80x25
   - `-cdrom mylinux.iso` → file ISO yang akan dijalankan

**Hasil**

- Layar akan menampilkan proses booting
- Terdapat menu **GRUB** yang menampilkan pilihan: `MyLinux`
- Setelah memilih, sistem akan memuat kernel dan `initrd`, lalu masuk ke **BusyBox shell**

**Pengujian di BusyBox Shell**

Setelah masuk shell, coba perintah berikut untuk menguji lingkungan sistem:

```bash
ls -la
ls
whoami
ps
```

Perintah tersebut akan:

- Menampilkan isi direktori (`ls`)
- Menampilkan user aktif (`whoami`)
- Menampilkan proses yang sedang berjalan (`ps`)

**Mengulang Emulasi**

Jika ingin menjalankan ulang:

1. Matikan QEMU dengan:

   ```bash
   pkill -f qemu
   ```

2. Jalankan kembali perintah qemu-system-x86_64 seperti di atas.
