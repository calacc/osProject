#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

void printMenuRegularFile()
{
	printf("You can execute the following operations:\n");
	printf("-n: file name;\n");
	printf("-d: dim/size;\n");
	printf("-h: number of hard links;\n");
	printf("-m: time of last modification;\n");
	printf("-a: access rights;\n");
	printf("-l: create symbolic link (please give a link name).\n");
}

void printMenuSymbolicLink()
{
	printf("You can execute the following operations:\n");
	printf("-n: link name;\n");
	printf("-l: delete link;\n");
	printf("-d: size of the link;\n");
	printf("-t: size of the target;\n");
	printf("-a: access rights.\n");
}

void getName(char *path)
{
	char name[30]; strcpy(name, path);
	if(strchr(name, '/')!=NULL)
		strcpy(name, strrchr(name, '/')+1);
	printf("%s\n", name);
}

void executeOperations(char path[])
{
	struct stat filestat;
	if(lstat(path, &filestat)<0)
	{
		printf("Eror: unable to stat \"%s\" ", path);
		return;
	}
	char operations[50];
	if(S_ISREG(filestat.st_mode))//regular file
	{
		printMenuRegularFile();
		scanf("%s", operations);
		
		for(int i=1; operations[i]; ++i)
		{
			switch(operations[i])
			{
				case 'n': 
				{
					getName(path);
					break;
				}
				case 'd': 
				{
					printf("Size of \"%s\": %lld\n", path, (long long)filestat.st_size);
					break;
				}
				case 'h':
				{
					printf("Number of hard links of \"%s\": %d\n", path, (int)filestat.st_nlink);
					break;
				}
				case 'm': 
				{
					printf("Time of last modification of \"%s\": %s", path, ctime(&filestat.st_mtime));
					break;
				}
				case 'a': 
				{
					printf("Access rights of %s: ", path);
                    printf((filestat.st_mode & S_IRUSR) ? "r" : "-");
                    printf((filestat.st_mode & S_IWUSR) ? "w" : "-");
                    printf((filestat.st_mode & S_IXUSR) ? "x" : "-");
                    printf((filestat.st_mode & S_IRGRP) ? "r" : "-");
                    printf((filestat.st_mode & S_IWGRP) ? "w" : "-");
                    printf((filestat.st_mode & S_IXGRP) ? "x" : "-");
                    printf((filestat.st_mode & S_IROTH) ? "r" : "-");
                    printf((filestat.st_mode & S_IWOTH) ? "w" : "-");
                    printf((filestat.st_mode & S_IXOTH) ? "x" : "-");
                    printf("\n");
					break;
				}
				case 'l': 
				{
					char linkname[20];
					printf("Please provide a link name: ");
					scanf("%s", linkname);
					if(symlink(path, linkname)<0)
					{
						printf("Error: unable to create symbolic link \"%s\"", linkname);
						break;
					}
					printf("Symbolic link \"%s\" was created successfully", linkname);
					break;
				}
				default: printf("%c is not a valid operation", operations[i]);
			}
		}
		
		return;
	}
	if(S_ISLNK(filestat.st_mode))//symbolic link
	{
		printMenuSymbolicLink();		
		scanf("%s", operations);
		
		for(int i=1; operations[i]; ++i)
		{
			switch(operations[i])
			{
				case 'n': 
				{
					getName(path);
					break;
				}
				case 'l':
				{
					if(unlink(path)<0)
						printf("Deletion of symbolic link \"%s\" was unsuccessful\n", path);
					else
						printf("Successfully deleted the symbolik link \"%s\"\n", path);
					break;
				}
				case 'd': 
				{
					printf("Size of \"%s\": %lld\n", path, (long long)filestat.st_size);
					break;
				}
				case 't': 
				{
					struct stat targetstat;
					if(stat(path, &targetstat)<0)
						printf("Error: unable to stat target of \"%s\"\n", path);
					else
						printf("Size of target file: %lld\n", (long long) targetstat.st_size);
					break;
				}
				case 'a':
				{
					printf("Access rights of \"%s\": ", path);
                    printf((filestat.st_mode & S_IRUSR) ? "r" : "-");
                    printf((filestat.st_mode & S_IWUSR) ? "w" : "-");
                    printf((filestat.st_mode & S_IXUSR) ? "x" : "-");
                    printf((filestat.st_mode & S_IRGRP) ? "r" : "-");
                    printf((filestat.st_mode & S_IWGRP) ? "w" : "-");
                    printf((filestat.st_mode & S_IXGRP) ? "x" : "-");
                    printf((filestat.st_mode & S_IROTH) ? "r" : "-");
                    printf((filestat.st_mode & S_IWOTH) ? "w" : "-");
                    printf((filestat.st_mode & S_IXOTH) ? "x" : "-");
                    printf("\n");
					break;
				}
				default: printf("%c is not a valid operation!", operations[i]);
			}
		}
		
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
		char path[50];
		strcpy(path, argv[i]);
		executeOperations(path);
	}

}
 