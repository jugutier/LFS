#include "applications.h"

#include "fs.h"
void test1();
void test2();
void test3();
int testScript_main(int argc, char *argv[]){
	printf("-----------------------------------------------------\n");
	printf("-----------------------------------------------------\n");
	printf("testscript: Probando las funciones del filesystem\n");
	printf("******************************************************\n");
	printf("Corriendo pruebas de integridad\n");
	test1();
	test2();
	test3();
	printf("Pruebas de integridad terminadas\n");
	printf("******************************************************\n");
	printf("***1.Listar home luego de la inicializacion\n");
	char *home[] = {"ls" ,"/"};
	ls_main(2,home);//nothing to list
	printf("***2.Crear un directorio de nombre pepe\n");
	char *pepe[2] = {"ls","/pepe"};
	mkdir_main(2,pepe);
	printf("***3.Listar home con el directorio nuevo\n");
	ls_main(2,home);//list pepe
	printf("***4.Crear un archivo con texto de prueba\n");
	char *nanoTest [3] = {"nano","/pepe/a.txt","lorem ipsum dolor sit amet"};
	nano_main(3,nanoTest);
	printf("***5.Cambiar al directorio pepe\n");
	cd_main(2,pepe);
	printf("***6.Listar el directorio pepe que ahora contiene el archivo de texto\n");
	ls_main(2,pepe);//list a.txt
	char *archive[2] = {"cat","/pepe/a.txt"};
	printf("***7. Mostrar los contenidos del archivo de texto\n");
	cat_main(2,archive);//show lorem ipsum
	printf("-----------------------------------------------------\n");
	printf("--------------------------Fin------------------------\n");
}
int mkdir_main(int argc, char *argv[]){
	if(argc!=2){
		printf("mkdir: wrong number of parameters\n");
		return -1;
	}
	char * directoryName = argv[1];
	int retVal=0;
	retVal = makeDirectory(directoryName);
	if( retVal == -1){
		printf("mkdir: directory full\n");
	}
	return retVal;
}
int ls_main(int argc, char *argv[]){
	if(argc!=2){
		printf("ls: wrong number of parameters\n");
		return -1;
	}
	char * path = argv[1];
	char ** printString;
	int i =0;
	printString = list(path);
	printf("directory 0 : ..\n");
	for(i=1;i<MAX_DIRECTORY;i++){
		if(printString[i]!=NULL){
			printf("directory %d : %s\n",i,printString[i]);
		}else{
			break;
		}
	}
	if(i ==1){
		printf("ls: No files or directories\n");
	}
	return 0;
}
int cd_main(int argc, char *argv[]){
	if(argc!=2){
		printf("cd: wrong number of parameters\n");
		return -1;
	}
	char * path = argv[1];
	printf("cd: new working directory = %s\n",path );
	return 0;
}

int nano_main(int argc, char *argv[]){
	printf("nano\n");
	if(argc!=3){
		printf("nano: wrong number of parameters\n");
		return -1;
	}
	char * path = argv[1];
	char * text = argv[2];
	//printf("path = %s text = %s\n",path,text );
	int filedes = open(path,O_RDWR |O_CREAT);
	//printf("filedes = %d\n",filedes );
	write(filedes,text,strlen(text));
	close(filedes);
	//Block * b = getBlockByInode(filedes);
	//printf("nano end %s\n",b->data);
	return 0;
}

int cat_main(int argc, char *argv[]){
	printf("cat\n");
	if(argc!=2){
		printf("cat: wrong number of parameters\n");
		return -1;
	}
	char * path = argv[1];
	printf("path = %s\n",path );
	char text [MAX_DATA];
	int readData = 0;
	int filedes = open(path,O_RDWR);
	readData = read(filedes,text,MAX_DATA);
	printf("Contents of %s : %s\n",path,text);
	close(filedes);
	return 0;
}

void test1(){
	int i;
	printf(">>>1.Creando un bloque vacio\n");
	Block * b = createEmptyBlock();
	Directory d[MAX_DIRECTORY];
	getDirectories(b, &d);
	printf("Recuperando directorios...\n");
	for(i=0; i<MAX_DIRECTORY ;i++){
		printf("se recupero el directorio %d\n",i);
	}
		//printf("-1 = %d %p\n",d[i].iNodeLocation ,d+i*sizeof(Directory) );
	printf(">>>test1 OK\n");
}
void test2(){
	printf(">>>2.Testear el correcto parseo de los path\n");
	char buff[MAX_FILENAME_SIZE];
	printf("1-con el path: /pepe\n");
	getFilename("/pepe",buff);
	printf("nombre del directorio = %s\n", buff);
	getParentDirectoryName("/pepe",buff);
	printf("nombre del padre = %s\n", buff);
	printf("2-Con el archivo: /pepe/a.txt\n");
	getFilename("/pepe/a.txt",buff);
	printf("nombre del archivo = %s\n", buff);
	getParentDirectoryName("/pepe/a.txt",buff);
	printf("nombre del padre = %s\n", buff);
	printf(">>>test2 OK\n");
}
void test3(){
	printf(">>>3.Prueba de sobrecarga del extent\n");
	Block * b;
	int i;
	printf("Creando bloques\n");
	for(i=0;i<EXTENT_BLOCKS;i++){
		b = createEmptyBlock();
		printf("bloque %d copiado\n",i );
		if(i%3 == 0){
			printf("vaciando extent\n");
		}
	}	
	printf(">>>test 3 OK\n");
}