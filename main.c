#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void printMenuRegularFile()
{
	printf("You can execute the following operations:\n");
	printf("-n: file name\n");
	printf("-d: dim/size\n");
	printf("-h: number of hard links\n");
	printf("-m: time of last modification\n");
	printf("-a: access rights\n");
	printf("-l: create symbolic link (please give a link name)\n");
}

void printMenuSymbolicLink()
{
	printf("You can execute the following operations:\n");
	printf("-n: link name\n");
	printf("-l: delete link\n");
	printf("-d: size of the link\n");
	printf("-t: size of the target\n");
	printf("-a: access rights\n");
}

void executeOperations(char path[])
{
	struct stat filestat;
	if(lstat(path, &filestat)<0)
	{
		printf("Eror: unable to stat %s. ", path);
		return;
	}
	char operations[50];
	if(S_ISREG(filestat.st_mode))//regular file
	{
		printMenuRegularFile();
		
		scanf("%s", operations);
		return;
	}
	if(S_ISLNK(filestat.st_mode))//symbolic link
	{
		printMenuSymbolicLink();
		
		scanf("%s", operations);
		return;
	}
	if(S_ISDIR(filestat.st_mode))//directory
	{
		
		return;
	}
}

int main(int argc, char* argv[])
{
	for(int i=1; i<argc; ++i)
	{
		char path[50], operations[50];
		strcpy(path, argv[i]);
		executeOperations(path);
	}
}