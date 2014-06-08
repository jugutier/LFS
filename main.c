#include "diskDriver/driver.h"
#include "fs/fs.h"

int main(void){

	if(!initDisk()){
		printf("failed while initDisk\n");
		return -1;
	}
	if(!initFS()){
		printf("failed while initFS\n");
		return -1;
	}
	ls();//nothing to list
	mkdir("pepe");
	cd("pepe");
	nano("a.txt","lorem ipsum dolor sit amet");
	ls();//list a.txt
	cd("..");
	ls();//list pepe
}