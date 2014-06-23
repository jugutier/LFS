#include "applications.h"

#include "fs.h"

int mkdir_main(int argc, char *argv[]){
	if(argc!=1){
		printf("mkdir: wrong number of parameters\n");
		return -1;
	}
	char * directoryName = *argv;
	int retVal=0;
	retVal = makeDirectory(directoryName);
	if( retVal == -1){
		printf("mkdir: directory full\n");
	}
	return retVal;
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
		if(printString[i]!=NULL){
			printf("directory %d : %s\n",i,printString[i]);
		}else{
			break;
		}
	}
	if(i ==0){
		printf("ls: No files or directories\n");
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
	printf("nano\n");
	if(argc!=2){
		printf("nano: wrong number of parameters\n");
		return -1;
	}
	char * path = argv[0];//*argv;
	char * text = argv[1];//*(argv+1*sizeof(char *));
	printf("path = %s text = %s\n",path,text );
	int filedes = open(path,O_RDWR |O_CREAT);
	printf("filedes = %d\n",filedes );
	write(filedes,text,strlen(text));
	close(filedes);
	Block * b = getBlockByInode(filedes);
	printf("nano end %s\n",b->data);
	return 0;
}

int cat_main(int argc, char *argv[]){
	printf("cat\n");
	if(argc!=1){
		printf("cat: wrong number of parameters\n");
		return -1;
	}
	char * path = *argv;
	printf("path = %s\n",path );
	char text [MAX_DATA];
	int readData = 0;
	int filedes = open(path,O_RDWR);
	readData = read(filedes,text,MAX_DATA);
	printf("Contents of %s : %s\n",path,text);
	close(filedes);
	return 0;
}