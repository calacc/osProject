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

void executeOperations(char s[])
{
	char path[50];
	strcpy(path, s);
	struct stat filestat;
	pid_t pid, pid1, pid2, pid3;
	int wstatus;
	pid_t w;
	int pipefd[2];
	
	if(lstat(path, &filestat)<0)
	{
		printf("Eror: unable to stat \"%s\" ", path);
		return;
	}
	
	if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }
	
	printf("\nName: ");
	getName(path);
	int count=0;
	
	if(S_ISREG(filestat.st_mode))
	{
		if(path[strlen(path)-2]=='.' && path[strlen(path)-1]=='c')
		{
			if((pid1=fork())<0)
			{
				printf("\nFailed to create the child process responsible with running the script.\n");
				exit(1);
			}
			count++;
			if(pid1==0)
			{
				// printf("\nChild process with PID %d created for the child process responsible with running the script.\n", getpid());
				close(pipefd[0]);
				dup2(pipefd[1], STDOUT_FILENO);
				
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
				close(pipefd[1]);
				ssize_t num_read;
				char buffer[50];
				int errors, warnings;
				while ((num_read = read(pipefd[0], buffer, sizeof(buffer))) > 0) {
					sscanf(buffer, "number of errors: %d\nnumber of warnings: %d", &errors, &warnings);

					int s = score(errors, warnings);
					
					FILE *output_file = fopen("grades.txt", "w");
					fprintf(output_file, "%s: %d\n", path, s);
					
					fclose(output_file);
				}
			}
		}
		else
		{
			if((pid1=fork())<0)
			{
				printf("\nFailed to create the child process responsible with counting the lines of a file.\n");
				exit(1);
			}
			count++;
			if(pid1==0)
			{
				// printf("\nChild process with PID %d created for the child process responsible with counting the lines of a file.\n", getpid());
				close(pipefd[0]);
				
				printf("The file %s has %d lines.\n", path, numberOfLines(path));
				
				exit(0);
			}
		}
	}
	if(S_ISDIR(filestat.st_mode))
	{
		if((pid2=fork())<0)
		{
			printf("\nFailed to create the child process responsible with creating a new text file.\n");
			exit(2);
		}
		count++;
		if(pid2==0)
		{
			// printf("\nChild process with PID %d created for the child process responsible with creating a new text file.\n", getpid());
			char fileName[50];
			strcpy(fileName, path);
			strcat(fileName, "_file.txt");
			char *arguments[] = {"touch", fileName, NULL};
			printf("Creating a text file with the name %s.\n", fileName);
			if(execv("/usr/bin/touch", arguments)==-1)
			{
				perror("execv");
				exit(EXIT_FAILURE);
			}
			exit(0);
		}
	}
	if(S_ISLNK(filestat.st_mode))
	{
		if((pid3=fork())<0)
		{
			printf("\nFailed to create the child process responsible with changing the permissions of the link.\n");
			exit(3);
		}
		count++;
		if(pid3==0)
		{
			// printf("\nChild process with PID %d created for the child process responsible with changing the permissions of the link.\n", getpid());
			char *arguments[] = {"chmod", "-v", "760", path, NULL};
			printf("Changing the permissions of the link %s.\n", path);
			if(execv("/usr/bin/chmod", arguments)==-1)
			{
				perror("execv");
				exit(EXIT_FAILURE);
			}
			exit(0);
		}
	}
	
	if((pid=fork()) < 0)
	{
		printf("\nFailed to create the child process responsible with the menu.\n");
		exit(4);
	}
	count++;

	if(pid==0)
	{
		// printf("\nChild process with PID %d created for the child process responsible with the menu.\n", getpid());
		if(S_ISREG(filestat.st_mode))//regular file
		{
			printf("\nFile type: regular file.\n");
			
			regularFileOperations(path);
			
			exit(0);
		}
		if(S_ISLNK(filestat.st_mode))//symbolic link
		{
			printf("\nFile type: symbolic link.\n");
			
			symbolicLinkOperations(path);
			
			exit(0);
		}
		if(S_ISDIR(filestat.st_mode))//directory
		{
			printf("\nFile type: directory.\n");
			
			directoryOperations(path);

			exit(0);
		}
	}
	
	printf("PID count: %d\n", count);
	for(int i=0; i<count; ++i)
	{
		w=wait(&wstatus);
		if (w == -1) 
		{
		    perror("waitpid");
		    exit(EXIT_FAILURE);
		}
		if (WIFEXITED(wstatus)) 
		{
		    printf("The process with PID %d has ended with the exit code %d.\n", w, WEXITSTATUS(wstatus));
		}
	}	
	count=0;
}

int main(int argc, char* argv[])
{
	for(int i=1; i<argc; ++i)
	{
		char path[50];
		strcpy(path, argv[i]);
		executeOperations(strdup(path));
	}
}
 