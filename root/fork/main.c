#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void)
{
	if (fork() == 0)
	{
		wait(NULL);
		printf("I am the child.\n");
	}
	else
	{
		printf("I am the parent.\n");
	}
}
