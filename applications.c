#include "applications.h"

#include "fs/fs.h"

int mkdir_main(int argc, char *argv[]){
	if(argc!=1){
		printf("mkdir: wrong number of parameters\n");
		return -1;
	}
	char * directoryName = *argv;

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
		printf("directory &d : %s\n",i,printString[i] );
	}
	return 0;
}

//bool cd(char * dirName);
int nano_main(int argc, char *argv[]){
//void nano(char * filename,char * text){



}