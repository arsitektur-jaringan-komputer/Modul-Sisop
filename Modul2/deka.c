#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

void abjad_shift_1() {
  int i;
  for (i = 65; i < 78; i++) {
    printf("%c\n", i);
  }
}

void abjad_shift_2() {
  int i;
  for (i = 78; i < 91; i++) {
    printf("%c\n", i);
  }
}

int main() {
  pid_t child_id;

  child_id = fork();
  
  if (child_id < 0) {
    exit(EXIT_FAILURE);
  }

  if (child_id == 0) {
    abjad_shift_1();
  } else {
    abjad_shift_2();
  }
}
