#include "diskDriver/driver.h"
#include "fs/fs.h"

int main(void){
	if(!initDisk()){
		printf("failed while initDisk\n");
		return -1;
	}
	printf("Init disk ok\n");
	if(!initFS()){
		printf("failed while initFS\n");
		return -1;
	}
	printf("Init fs ok\n");
	ls_main(1,"/");//nothing to list
	//makedirectory("pepe");
	//cd("pepe");
	//nano_main("a.txt","lorem ipsum dolor sit amet");
	list("/");//list a.txt
	//cd("..");
	//list();//list pepe
}
