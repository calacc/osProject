char name[50];

void getName(char *path, char fileName[])
{
	char name[30]; strcpy(name, path);
	if(strchr(name, '/')!=NULL)
		strcpy(name, strrchr(name, '/')+1);
	printf("%s\n", name);
	strcpy(fileName, name);
}

void printMenuRegularFile()
{
	printf("\nYou can execute the following operations:\n");
	printf("-n: file name;\n");
	printf("-d: dim/size;\n");
	printf("-h: number of hard links;\n");
	printf("-m: time of last modification;\n");
	printf("-a: access rights;\n");
	printf("-l: create symbolic link (please give a link name).\n");
}

void printMenuSymbolicLink()
{
	printf("\nYou can execute the following operations:\n");
	printf("-n: link name;\n");
	printf("-l: delete link;\n");
	printf("-d: size of the link;\n");
	printf("-t: size of the target;\n");
	printf("-a: access rights.\n");
}

void printMenuDirectory()
{
	printf("\nYou can execute the following operations:\n");
	printf("-n: directory name;\n");
	printf("-d: size;\n");
	printf("-a: access rights;\n");
	printf("-c: total number of files with the \".c\" extension.\n");
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
				getName(path, name);
				break;
			}
			case 'd': 
			{
				printf("Size of \"%s\": %lld\n", name, (long long)filestat.st_size);
				break;
			}
			case 'h':
			{
				printf("Number of hard links of \"%s\": %d\n", name, (int)filestat.st_nlink);
				break;
			}
			case 'm': 
			{
				printf("Time of last modification of \"%s\": %s", name, ctime(&filestat.st_mtime));
				break;
			}
			case 'a': 
			{
				printf("Access rights of %s: \n", name);
				printf("User:\n\n");
				printf((filestat.st_mode & S_IRUSR) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWUSR) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXUSR) ? "Exec - yes\n" : "Exec - no\n");
				printf("\nGroup:\n\n");
				printf((filestat.st_mode & S_IRGRP) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWGRP) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXGRP) ? "Exec - yes\n" : "Exec - no\n");
				printf("\nOthers:\n\n");
				printf((filestat.st_mode & S_IROTH) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWOTH) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXOTH) ? "Exec - yes\n" : "Exec - no\n");
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
				getName(path, name);
				break;
			}
			case 'l':
			{
				if(unlink(path)<0)
					printf("Deletion of symbolic link \"%s\" was unsuccessful\n", name);
				else
					printf("Successfully deleted the symbolik link \"%s\"\n", name);
				return;
				break;
			}
			case 'd': 
			{
				printf("Size of \"%s\": %lld\n", name, (long long)filestat.st_size);
				break;
			}
			case 't': 
			{
				struct stat targetstat;
				if(stat(path, &targetstat)<0)
					printf("Error: unable to stat target of \"%s\"\n", name);
				else
					printf("Size of target file: %lld\n", (long long) targetstat.st_size);
				break;
			}
			case 'a':
			{
				printf("Access rights of %s: \n", name);
				printf("User:\n\n");
				printf((filestat.st_mode & S_IRUSR) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWUSR) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXUSR) ? "Exec - yes\n" : "Exec - no\n");
				printf("\nGroup:\n\n");
				printf((filestat.st_mode & S_IRGRP) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWGRP) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXGRP) ? "Exec - yes\n" : "Exec - no\n");
				printf("\nOthers:\n\n");
				printf((filestat.st_mode & S_IROTH) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWOTH) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXOTH) ? "Exec - yes\n" : "Exec - no\n");
				
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
				getName(path, name);
				break;
			}
			case 'd':
			{
				printf("Size of \"%s\": %lld\n", name, (long long)filestat.st_size);
				break;
			}
			case 'a':
			{
				printf("Access rights of %s: \n", name);
				printf("User:\n\n");
				printf((filestat.st_mode & S_IRUSR) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWUSR) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXUSR) ? "Exec - yes\n" : "Exec - no\n");
				printf("\nGroup:\n\n");
				printf((filestat.st_mode & S_IRGRP) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWGRP) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXGRP) ? "Exec - yes\n" : "Exec - no\n");
				printf("\nOthers:\n\n");
				printf((filestat.st_mode & S_IROTH) ? "Read - yes\n" : "Read - no\n");
				printf((filestat.st_mode & S_IWOTH) ? "Write - yes\n" : "Write - no\n");
				printf((filestat.st_mode & S_IXOTH) ? "Exec - yes\n" : "Exec - no\n");
				
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
					printf("There are %d \".c\" files in the directory %s.\n", count, name);
				} else {
					printf("Could not open directory %s!\n", name);
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

int numberOfLines(char fileName[])
{
	FILE *fin = fopen(fileName, "r");
	
	int counter=1;
	char c;
	while((c=fgetc(fin))>0)
	{
		if(c=='\n')
			counter++;
	}
	fclose(fin);
	
	return counter;	
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


