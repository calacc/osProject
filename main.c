#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include "functions.c"

void getName(char *path)
{
	char name[30]; strcpy(name, path);
	if(strchr(name, '/')!=NULL)
		strcpy(name, strrchr(name, '/')+1);
	printf("%s\n", name);
}

void regularFileOperations(char path[])
{
	struct stat filestat;
	
	if(lstat(path, &filestat)<0)
	{
		printf("Eror: unable to stat \"%s\" ", path);
		return;
	}
	
	char operations[50];
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
				printf("User:\n\n");
				printf((filestat.st_mode & S_IRUSR) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWUSR) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXUSR) ? "Execute - yes\n" : "Execute - no\n");
				printf("\nGroup:\n\n");
				printf((filestat.st_mode & S_IRGRP) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWGRP) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXGRP) ? "Execute - yes\n" : "Execute - no\n");
				printf("\nOthers:\n\n");
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
			default: 
			{
				printf("%c is not a valid operation", operations[i]);
				regularFileOperations(path);
			}
		}
	}
}

void symbolicLinkOperations(char path[])
{
	struct stat filestat;
	
	if(lstat(path, &filestat)<0)
	{
		printf("Eror: unable to stat \"%s\" ", path);
		return;
	}
	
	char operations[50];
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
				return;
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
				printf("User:\n\n");
				printf((filestat.st_mode & S_IRUSR) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWUSR) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXUSR) ? "Execute - yes\n" : "Execute - no\n");
				printf("\nGroup:\n\n");
				printf((filestat.st_mode & S_IRGRP) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWGRP) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXGRP) ? "Execute - yes\n" : "Execute - no\n");
				printf("\nOthers:\n\n");
				printf((filestat.st_mode & S_IROTH) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWOTH) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXOTH) ? "Execute - yes\n" : "Execute - no\n");
				
				break;
			}
			default: 
			{
				printf("%c is not a valid operation!", operations[i]);
				symbolicLinkOperations(path);
			}
		}
	}
	
}

void directoryOperations(char path[])
{
	struct stat filestat;
	
	if(lstat(path, &filestat)<0)
	{
		printf("Eror: unable to stat \"%s\" ", path);
		return;
	}
	
	char operations[50];
	
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
				printf("User:\n\n");
				printf((filestat.st_mode & S_IRUSR) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWUSR) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXUSR) ? "Execute - yes\n" : "Execute - no\n");
				printf("\nGroup:\n\n");
				printf((filestat.st_mode & S_IRGRP) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWGRP) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXGRP) ? "Execute - yes\n" : "Execute - no\n");
				printf("\nOthers:\n\n");
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
			default: 
			{
				printf("%c is not a valid operation!", operations[i]);
				directoryOperations(path);
			}
		}
	}
	
}

int score(int errors, int warnings)
{
	if(errors==0 && warnings==0)
		return 10;
	if(errors>0)
		return 1;
	if(warnings>10)
		return 2;
	return 2+8*(10-warnings)/10;
}

void executeOperations(char path[])
{
	struct stat filestat;
	pid_t pid, pid1, pid2;
	int wstatus;
	pid_t w;
	// int pipefd[2];
	
	if(lstat(path, &filestat)<0)
	{
		printf("Eror: unable to stat \"%s\" ", path);
		return;
	}
	printf("Name: ");
	getName(path);
	int count=0;
	
	if(S_ISREG(filestat.st_mode))
	{
		if(path[strlen(path)-2]=='.' && path[strlen(path)-1]=='c')
		{
			if((pid1=fork())<0)
			{
				printf("Failed to create pid1 child process.\n");
				exit(1);
			}
			count++;
			if(pid1==0)
			{
				// close(pipefd[0]);
				// dup2(pipefd[1], STDOUT_FILENO);
				
				char *arguments[] = {"bash", "script.sh", path, "error.txt", NULL};
				printf("This is a .c file. Executing script 'script.sh'\n");
				if(execv("/usr/bin/bash", arguments)==-1)
				{
					perror("execv");
					exit(EXIT_FAILURE);
				}
				exit(0);
			}
		}
	}
	if(S_ISDIR(filestat.st_mode))
	{
		if((pid2=fork())<0)
		{
			printf("Failed to create pid2 child process.\n");
			exit(1);
		}
		count++;
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
	}

	
	if((pid=fork()) < 0)
	{
		printf("Failed to create the child process.\n");
		exit(1);
	}
	count++;

	if(pid==0)
	{
		
		if(S_ISREG(filestat.st_mode))//regular file
		{
			printf("File type: regular file.\n");
			
			regularFileOperations(path);
			
			return;
		}
		if(S_ISLNK(filestat.st_mode))//symbolic link
		{
			printf("File type: symbolic link.\n");
			
			symbolicLinkOperations(path);
			
			return;
		}
		if(S_ISDIR(filestat.st_mode))//directory
		{
			printf("File type: directory.\n");
			
			directoryOperations(path);

			return;
		}
	}
	for(int i=0; i<count; i++)
	{
		w=wait(&wstatus);
		if (w == -1) 
		{
		    perror("waitpid");
		    exit(EXIT_FAILURE);
		}
		if (WIFEXITED(wstatus)) 
		{
		    printf("exited child process with id%d, status=%d\n", w, WEXITSTATUS(wstatus));
		}
		
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
 