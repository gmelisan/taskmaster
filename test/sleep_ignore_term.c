#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
	int ret;

	signal(SIGTERM, SIG_IGN);
	if (argc > 1) {
		while ((ret = sleep(atoi(argv[1])) != 0)) {
			sleep(ret);
		}
		return 0;
	}
	return 1;
}
