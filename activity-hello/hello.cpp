#include <stdio.h>
#include <unistd.h>

#define HOST_NAME_LENGTH 128

int main()
{
	char hostname[HOST_NAME_LENGTH];

	gethostname(hostname, HOST_NAME_LENGTH);
    
	printf("Host Machine Name: %s\n", hostname);

	return 0;
}
