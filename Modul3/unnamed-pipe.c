#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	int pfds[2];
	pipe(pfds);
	if ( !fork() ) {
		close(1);
		dup(pfds[1]);
		close(pfds[0]);
		execlp("ls", "ls", NULL);
	} else {
		close(0);
		dup(pfds[0]);
		close(pfds[1]);
		char *grep[] = {"grep", "pipe", NULL};
		execv("/bin/grep", grep);
	}
	return 0;
}
