#include "diskDriver/driver.h"
#include "fs/fs.h"
void test1();
void test2();
int main(void){
	//printf("Haciendo tests\n");
	//test1();
	//test2();
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
	char *path[] = {"/"};
	ls_main(1,path);//nothing to list
	char *pepe[] = {"pepe"};
	mkdir_main(1,pepe);
	//cd_main(1,"pepe");
	//nano_main(2,{"a.txt","lorem ipsum dolor sit amet"});
	//ls_main(1,"/");//list a.txt
	//cat_main(1,"pepe/a.txt");
	//cd_main(1,"..");
	//ls_main(1,"pepe");//list pepe
}
void test1(){
	int i;
	printf("Creando un bloque vacio\n");
	Block * b = createEmptyBlock();
	Directory d[MAX_DIRECTORY];
	getDirectories(b, &d);
	printf("recuperando directorios\n");
	for(i=0; i<MAX_DIRECTORY ;i++)
		printf("-1 = %d %p\n",d[i].iNodeLocation ,d+i*sizeof(Directory) );
	printf("test1 OK\n");
}
void test2(){
	char buff[MAX_FILENAME_SIZE];
	getFilename("/pepe/a.txt",buff);
	printf("filename = %s\n", buff);
	printf("test2 OK\n");
}
