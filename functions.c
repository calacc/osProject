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