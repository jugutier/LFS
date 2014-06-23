#include "applications.h"

#include "fs/fs.h"

int mkdir_main(int argc, char *argv[]){
	if(argc!=1){
		printf("mkdir: wrong number of parameters\n");
		return -1;
	}
	char * directoryName = *argv;
	makeDirectory(directoryName);
	return 0;
}
int ls_main(int argc, char *argv[]){
	if(argc!=1){
		printf("ls: wrong number of parameters\n");
		return -1;
	}
	char * path = *argv;
	char ** printString;
	int i =0;
	printString = list(path);
	for(i=0;i<MAX_DIRECTORY;i++){
		printf("directory %d : %s\n",i,printString[i] );
	}
	return 0;
}
int cd_main(int argc, char *argv[]){
	if(argc!=1){
		printf("cd: wrong number of parameters\n");
		return -1;
	}
	char * path = *argv;
	printf("path = %s\n",path );
	return 0;
}

int nano_main(int argc, char *argv[]){
	if(argc!=2){
		printf("nano: wrong number of parameters\n");
		return -1;
	}
	char * filename = *argv;
	char * text = *(argv+1*sizeof(char *));
	printf("filename = %s text = %s\n",filename,text );
	//int filedes = open(filename,O_RDWR |O_CREAT);
	//write(filedes,text,strlen(text));
	//close(filedes);
	return 0;
}

int cat_main(int argc, char *argv[]){
	if(argc!=1){
		printf("cat: wrong number of parameters\n");
		return -1;
	}
	char * path = *argv;
	printf("path = %s\n",path );
	char text [MAX_DATA];
	int readData = 0;
	//int filedes = open(filename,O_RDWR);
	//readData = read(filedes,text,MAX_DATA);
	//printf("Contents of %s : %s\n",path,text);
	//close(filedes);
	return 0;
}