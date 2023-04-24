#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
//for each file the name and type will be displayed si dupa afisezi meniu
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

void printMenuDirectory()
{
	printf("You can execute the following operations:\n");
	printf("-n: directory name;\n");
	printf("-d: size;\n");
	printf("-a: access rights;\n");
	printf("-c: total number of files with the \".c\" extension.\n");
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
	pid_t pid, pid1, pid2;
	
	if(lstat(path, &filestat)<0)
	{
		printf("Eror: unable to stat \"%s\" ", path);
		return;
	}
	printf("Name: ");
	getName(path);
	char operations[50];
	
	if((pid=fork()) < 0)
	{
		printf("Failed to create the child process.\n");
		exit(1);
	}
	
	if(pid==0)
	{
		
		if(S_ISREG(filestat.st_mode))//regular file
		{
			printf("File type: regular file.\n");
			
			if(path[strlen(path)-2]=='.' && path[strlen(path)-1]=='c')
			{
				if((pid1=fork())<0)
				{
					printf("Failed to create pid1 child process.\n");
					exit(1);
				}
				if(pid1==0)
				{
					char *arguments[] = {"bash", "script.sh", path, "error.txt", NULL};
					printf("This is a .c file. Executing script 'script.sh'\n");
					if(execv("/usr/bin/bash", arguments)==-1)
					{
						perror("execv");
						exit(EXIT_FAILURE);
					}
					exit(0);
				}
				else
				{
					wait(NULL);
					printf("Done! Sending you to regular file menu!\n");
				}
			}
			
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
						printf("Access rights of %s: \n", path);
						printf("User:\n");
						printf((filestat.st_mode & S_IRUSR) ? "Read - yes\n" : "Read - no\n");
						printf((filestat.st_mode & S_IWUSR) ? "Write - yes\n" : "Write - no\n");
						printf((filestat.st_mode & S_IXUSR) ? "Execute - yes\n" : "Execute - no\n");
						printf("Group:\n");
						printf((filestat.st_mode & S_IRGRP) ? "Read - yes\n" : "Read - no\n");
						printf((filestat.st_mode & S_IWGRP) ? "Write - yes\n" : "Write - no\n");
						printf((filestat.st_mode & S_IXGRP) ? "Execute - yes\n" : "Execute - no\n");
						printf("Others:\n");
						printf((filestat.st_mode & S_IROTH) ? "Read - yes\n" : "Read - no\n");
						printf((filestat.st_mode & S_IWOTH) ? "Write - yes\n" : "Write - no\n");
						printf((filestat.st_mode & S_IXOTH) ? "Execute - yes\n" : "Execute - no\n");
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
			printf("File type: symbolic link.\n");
			
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
						printf("Access rights of %s: \n", path);
						printf("User:\n");
						printf((filestat.st_mode & S_IRUSR) ? "Read - yes\n" : "Read - no\n");
						printf((filestat.st_mode & S_IWUSR) ? "Write - yes\n" : "Write - no\n");
						printf((filestat.st_mode & S_IXUSR) ? "Execute - yes\n" : "Execute - no\n");
						printf("Group:\n");
						printf((filestat.st_mode & S_IRGRP) ? "Read - yes\n" : "Read - no\n");
						printf((filestat.st_mode & S_IWGRP) ? "Write - yes\n" : "Write - no\n");
						printf((filestat.st_mode & S_IXGRP) ? "Execute - yes\n" : "Execute - no\n");
						printf("Others:\n");
						printf((filestat.st_mode & S_IROTH) ? "Read - yes\n" : "Read - no\n");
						printf((filestat.st_mode & S_IWOTH) ? "Write - yes\n" : "Write - no\n");
						printf((filestat.st_mode & S_IXOTH) ? "Execute - yes\n" : "Execute - no\n");
						break;
					}
					default: printf("%c is not a valid operation!", operations[i]);
				}
			}
			
			return;
		}
		if(S_ISDIR(filestat.st_mode))//directory
		{
			printf("File type: directory.\n");
			
			if((pid2=fork())<0)
			{
				printf("Failed to create pid2 child process.\n");
				exit(1);
			}
			
			if(pid2==0)
			{
				char *arguments[] = {"touch", "newFile.txt", NULL};
				printf("Creating a text file with the name 'newFile.txt'.\n");
				if(execv("/usr/bin/touch", arguments)==-1)
				{
					perror("execv");
					exit(EXIT_FAILURE);
				}
				exit(0);
			}
			else
			{
				wait(NULL);
				printf("Done! Sending you to directory menu!\n");
			}
			
			printMenuDirectory();
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
					case 'a':
					{
						printf("Access rights of %s: \n", path);
						printf("User:\n");
						printf((filestat.st_mode & S_IRUSR) ? "Read - yes\n" : "Read - no\n");
						printf((filestat.st_mode & S_IWUSR) ? "Write - yes\n" : "Write - no\n");
						printf((filestat.st_mode & S_IXUSR) ? "Execute - yes\n" : "Execute - no\n");
						printf("Group:\n");
						printf((filestat.st_mode & S_IRGRP) ? "Read - yes\n" : "Read - no\n");
						printf((filestat.st_mode & S_IWGRP) ? "Write - yes\n" : "Write - no\n");
						printf((filestat.st_mode & S_IXGRP) ? "Execute - yes\n" : "Execute - no\n");
						printf("Others:\n");
						printf((filestat.st_mode & S_IROTH) ? "Read - yes\n" : "Read - no\n");
						printf((filestat.st_mode & S_IWOTH) ? "Write - yes\n" : "Write - no\n");
						printf((filestat.st_mode & S_IXOTH) ? "Execute - yes\n" : "Execute - no\n");
						break;
					}
					case 'c':
					{
						DIR *dir;
						struct dirent *ent;
						int count=0;
						if ((dir = opendir (path)) != NULL) {
							while ((ent = readdir (dir)) != NULL) {
								int n=strlen(ent->d_name);
								char name[20];
								strcpy(name, ent->d_name);
								if(name[n-1]=='c'&& name[n-2]=='.')
									count++;
							}
							closedir (dir);
							printf("There are %d \".c\" files in the directory.\n", count);
						} else {
							printf("Could not open directory!\n");
						}
						break;
					}
					default: printf("%c is not a valid operation!", operations[i]);
				}
			}
			return;
		}
	}
	else
	{
		wait(NULL);
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
 