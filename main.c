#include "diskDriver/driver.h"
#include "fs/fs.h"
void test1();
void test2();
void test3();
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
	printf("Running tests\n");
	//test1();
	//test2();
	//test3();
	printf("Finished tests\n");
	char *path[] = {"/"};
	ls_main(1,path);//nothing to list
	char *pepe[] = {"/pepe"};
	mkdir_main(1,pepe);
	ls_main(1,path);//list pepe
	//cd_main(1,"pepe");
	char *nanoTest [2] = {"/pepe/a.txt","lorem ipsum dolor sit amet"};
	nano_main(2,nanoTest);
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
	printf("testing folder: /pepe\n");
	getFilename("/pepe",buff);
	printf("foldername = %s\n", buff);
	getParentDirectoryName("/pepe",buff);
	printf("parent = %s\n", buff);
	printf("testing file: /pepe/a.txt\n");
	getFilename("/pepe/a.txt",buff);
	printf("filename = %s\n", buff);
	getParentDirectoryName("/pepe/a.txt",buff);
	printf("parent = %s\n", buff);
	printf("test2 OK\n");
}
void test3(){
	Block * b;
	int i;
	for(i=0;i<EXTENT_BLOCKS;i++){
		b = createEmptyBlock();
		printf("block %d\n",i );
		saveToExtent(b);
	}
}