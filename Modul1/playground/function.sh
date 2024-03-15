#!/bin/bash

# deklarasi fungsi
function ask_name() {
  echo "Siapa namamu?"
}

say_hello() {
  read nama
  echo "Hai $nama, selamat datang di praktikum sistem operasi!"
}

# pemanggilan fungsi
ask_name
say_hello
