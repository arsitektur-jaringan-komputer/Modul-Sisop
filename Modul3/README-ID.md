# Modul 1 - Pengenalan Linux

## Capaian

1. Memahami kontainerisasi dan perbedaannya dengan virtualisasi
2. Memahami arsitektur dasar docker
3. Dapat melakukan bind file system dengan docker

## Prasyarat

1. Melakukan instalasi docker, bisa mengikuti modul [ini](https://github.com/arsitektur-jaringan-komputer/Pelatihan-Docker/tree/master/0.%20Prasyarat)

## Daftar Isi

- [i. Capaian](#capaian)
- [ii. Prasyarat](#prasyarat)
- [iii. Daftar Isi](#daftar-isi)
- [1. Pendahuluan](#pendahuluan)
  - [1.1. Mengapa Perlu Isolasi Proses?](#mengapa-perlu-isolasi-proses?)
  - [1.2. Perbedaan Kontainerisasi dan Virtualisasi](#perbedaan-kontainerisasi-dan-virtualisasi)
  - [1.3. Pengertian Docker](#pengertian-docker)
- [2. Arsitektur Docker](#arsitektur-docker)
  - [2.1. Docker Daemon](#docker-daemon)
  - [2.2. Docker Client](#docker-client)
  - [2.3. Docker Objects](#docker-objects)
  - [2.4. Docker Registry](#docker-registry)
- [3. Docker Service Dasar](#docker-service-dasar)
  - [3.1. Docker Image](#docker-image)
  - [3.2. Dockerfile](#dockerfile)
  - [3.3. Docker Container](#docker-container)
  - [3.4. Docker Hub](#docker-hub)
- [4. Docker Service Lanjutan](#docker-service-lanjutan)
  - [4.1. Docker Compose](#docker-compose)
  - [4.2. Docker Storage Management](#docker-storage-management)
- [iv. Referensi](#referensi)

## Pendahuluan

### Mengapa Perlu Isolasi Proses?

Pernahkah kamu mengompilasi program sambil mendengarkan musik MP3 di laptopmu? Kamu mungkin berasumsi programmu dan pemutar MP3 tidak akan berinteraksi. Namun, pada kenyataannya, keduanya mungkin berjalan pada prosesor yang persis sama. Jadi, bagaimana sistem memastikan bahwa program yang kamu kompilasi tidak pernah menimpa program pemutar MP3-mu? Mengapa program-program ini tidak saling memengaruhi?

Program-program tersebut berjalan di dalam proses yang berbeda, dimana setiap proses memiliki address space-nya sendiri. Address space inilah yang menyediakan isolasi proses.

Konsep dari isolasi proses yaitu apa pun yang dilakukan suatu proses tidak boleh memengaruhi atau mengganggu proses lainnya, kecuali jika proses tersebut setuju untuk berkomunikasi (misalnya, lewat pipe, mengacu kepada modul 2). 

Bayangkan jika satu proses gagal. Tanpa isolasi proses, kegagalan satu proses ini dapat merusak proses lainnya, bahkan dapat menyebabkan kegagalan seluruh sistem. Sangat berbahaya, bukan? Karena itulah sangat penting untuk melakukan isolasi proses.

### Perbedaan Kontainerisasi dan Virtualisasi

Sekarang kita akan berkenalan dengan kontainerisasi dan virtualisasi. 

Kontainerisasi adalah metode untuk mengemas aplikasi dan dependensinya ke dalam sebuah wadah (kontainer) yang dapat dijalankan secara konsisten di berbagai lingkungan komputasi, tanpa perlu mengubah kode atau konfigurasi aplikasi itu sendiri. Kontainer merupakan unit yang portabel, ringan, dan dapat diisolasi, yang mengemas aplikasi, library, dan konfigurasi menjadi satu entitas yang dapat dijalankan di lingkungan yang berbeda, seperti lokal, cloud, atau pusat data. 

Sedangkan virtualisasi adalah teknologi yang memungkinkan pembuatan mesin virtual di dalam satu fisik server. Dengan menggunakan hypervisor, virtualisasi memungkinkan pengelolaan beberapa sistem operasi atau aplikasi yang berjalan secara mandiri. Konsep dasar virtualisasi melibatkan isolasi sumber daya antara mesin virtual, sehingga setiap mesin virtual dapat beroperasi seolah-olah menjadi mesin fisik yang terpisah.

![virtualization-vs-containerization](assets/virtualization-vs-containerization.jpg)

Baik virtualisasi dan kontainerisasi menggunakan isolasi sistem untuk menjalankan beberapa sistem operasi atau aplikasi secara terpisah pada satu perangkat keras yang sama. Namun, virtualisasi dan kontainerisasi memiliki perbedaan utama. Virtualisasi membuat salinan virtual dari seluruh komputer fisik, yang dapat memakan banyak sumber daya. Kontainerisasi hanya mengemas aplikasi dan dependensi mereka dalam kontainer di dalam host yang sama, berbagi kernel OS yang sama, sehingga lebih ringan dan lebih efisien daripada virtualisasi.

### Pengertian Docker

![docker-logo](assets/docker-logo.png)

Salah satu teknologi untuk membuat kontainer yang berbagi sistem operasi host adalah Docker. Dengan Docker, pengembang dapat membuat kontainer yang konsisten dan portabel, yang dapat dijalankan di berbagai lingkungan komputasi, termasuk mesin lokal, server cloud, atau lingkungan pengembangan dan produksi yang berbeda. Docker memungkinkan aplikasi dan dependensinya diisolasi, sehingga aplikasi dapat dijalankan secara konsisten di berbagai lingkungan tanpa mengganggu host operating system atau aplikasi lainnya.

## Arsitektur Docker

![architecture](assets/architecture.png)

### Docker Daemon

Docker Daemon adalah komponen yang berjalan di latar belakang (background) pada host dan bertanggung jawab untuk menjalankan dan mengelola Docker Object seperti images, container, network, dan lain-lain. Docker Daemon adalah proses yang berjalan di dalam sistem operasi host dan menerima perintah dari Docker Client untuk membuat, menjalankan, menghentikan, dan mengelola Docker Object. Docker Daemon juga bertanggung jawab untuk mengelola sumber daya host seperti CPU, memori, dan jaringan yang digunakan oleh Docker Object.

### Docker Client

Docker Client adalah antarmuka pengguna berbasis command-line atau GUI yang digunakan untuk berinteraksi dengan Docker. Docker Client memungkinkan pengguna untuk menjalankan perintah-perintah Docker untuk membuat, mengelola, dan mengontrol layanan pada Docker. Docker Client berkomunikasi dengan Docker Daemon untuk mengirimkan perintah-perintah Docker dan menerima output layanan Docker yang sedang berjalan.

### Docker Objects

Docker Objects adalah komponen dasar yang terdapat di Docker. Beberapa contoh Docker Objects meliputi image, container, volume, dan network yang akan dijelaskan pada modul selanjutnya.

### Docker Registry

Docker Registry adalah repositori yang digunakan untuk menyimpan dan berbagi Docker Image. Docker Registry berfungsi sebagai tempat penyimpanan untuk Docker Image yang dapat diakses oleh pengguna Docker dari berbagai lokasi. Docker Hub, yang merupakan Docker public registry, adalah salah satu contoh Docker Registry yang sering digunakan untuk menyimpan dan berbagi Docker Image secara publik. Selain Docker Hub, pengguna juga dapat membuat Docker Registry pribadi untuk menyimpan Docker Image.

## Docker Service Dasar

![docker-service-dasar](assets/docker-service-dasar.png)

### Dockerfile

### Docker Image

### Docker Container

### Docker Hub

## Docker Service Lanjutan

### Docker Compose

### Docker Storage Management

## Referensi

[Modul Pelatihan Docker](https://github.com/arsitektur-jaringan-komputer/Pelatihan-Docker)