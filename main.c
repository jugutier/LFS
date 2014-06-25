#include "driver.h"
#include "fs.h"

int main(void){	
	if(!initDisk()){
		printf("failed while initDisk\n");
		return -1;
	}
	if(!initFS()){
		printf("failed while initFS\n");
		return -1;
	}
	printf("Init ok\n");
	testScript_main(0,NULL);
	
}
