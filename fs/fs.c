#include "fs.h"
#include "../diskDriver/driver.h"

#define HOME_DIRECTORY 1//inode number for '/' the home directory
#define MAX_DATA 256
#define EXTENT_BLOCKS 5
#define MAX_DIRECTORY 13
/*typedef struct 
{
	it supports just one dataBlock for the moment
	void * dataBlock;
}iNode;*/

typedef struct{
	/*this should be a map to support multiple files per directory*/
	char fileName [16];
	int iNodeLocation;
}Directory;//sizeof will be MAX_DATA. or 13 pairs of filename + inode

/*typedef struct 
{
	int iNodeNumber;
	void * diskAddress;
}iMapEntry;*/

typedef struct 
{
	int iNodeNumber;
	bool isDirectory;
	char data[MAX_DATA];//text file or Directory as byte string
}block;//sizeof = 272

typedef struct{
	int imap [34];//key inodenumber,  disk sector positions
}CR;//sizeof =272

//this will be the cache, cleared with an external thread uppon fillup
static block[EXTENT_BLOCKS] extent;
static int currentExtentBlock = 0;
static CR * cr;
//used for the circular buffer:
static int currentDiskStart = 0;
static int currentDiskEnd = MAX_SECTOR;

bool saveCR();
void initHomeDirectory();
bool initFS(){
	if(cr!=NULL){
		cr = malloc(sizeof(CR));
	}
	saveCR();
	initHomeDirectory();
}

void saveCR(){
	writeDisk(currentDiskStart, currentDiskStart+1, cr);
}


Block * getBlock(char * pathname) {
	/*
	 *  
	 * Split pathname with "/" as a delimiter.
	 *check in the extent if path exists
	 * Goto root inode, and search in the directory for the 
	 * next relative path.
     */
     return NULL;
}

char * getFileName(char * pathname) {
	// Split pathname with "/" and return the last element
}
/*
* Inserts the block in the extent and return
* the extent position.
*/
int insertInFdTable(Block * block) {

	log(block);
	return currentExtentBlock;
}

int open(char * pathname, int flags){
	switch(flags){
		case  O_RDWR:
			block * block = getBlock(pathname);
			int fd = insertInFdTable(b); 
			return fd;
		break;
		case O_RDWR|O_CREAT:
				// file does not exist
			block * parentBlock = getParentBlock(pathname);
			char * filename = getFileName(pathname);
			block b;
			b.isDirectory = false;

			addFile(parentBlock, &b, filename); // if (... == true)
			int fd = insertInFdTable(b);
			return fd;
		break;
		case O_RDWR|O_TRUNC:

		break;
	}
}
int write(int fildes, const void *buf, int nbyte){
	/*
	* Get the extent at position fildes. 
	* Copy nbyte bytes from buf to block->data
	* If before writing 
	* strlen(block->data) + nbyte > MAX_DATA
	* return -1. 
	*/
}

int close(int fildes) {
	/*
	* Won't do anything.
	* In next release: invalidate fildes
	*/
	return 0;
}
void initHomeDirectory(){
	block * b = malloc(sizeof(block));
	b->isDirectory= true;
	Directory** subdirectoires;
	addEmptyDirectories(subdirectoires);
	memcpy(b->data,subdirectories, MAX_DATA);
	log(b);
}

void addEmptyDirectories(Directory ** listofDirectories){
	Directory newDirectories [MAX_DIRECTORY];//malloc
	listofDirectories = &newDirectories;
	for(int i = 0; i<MAX_DIRECTORY;i++){
		newDirectories[i]->iNodeLocation = -1;
	}
}

void makedirectory(char * dirName){
	/*
	* go to parent directory and add another directory into
	* list of directories. log parent.create a new block with
	* subdiretories for the new "folder". log child.
	*/
}
void log(block * block){
	/**
	*check if im running out of space and clear if needed
	**/
	if(currentExtentBlock == floor(EXTENT_BLOCKS*0.9)){
		writeLog();
	}
	memcpy(extent[currentExtentBlock++] , block,BLOCK_SIZE) ;
}
//persists extent to disk, and clears it;
void writeLog(){
	//updateCR for each block
	for (int i = 0; i < EXTENT_BLOCKS; ++i)
	{
		writeDisk(currentDiskEnd+i, currentDiskEnd+i+1, extent[i]);
		memset(extent[i],0,BLOCK_SIZE);
	}
	currentExtentBlock = 0;
}
//return list of strings with names or null if not a directory
char** list(char * path){
	/*getBlock of that path,if its not a diretory return null,
	 save list subdirectoires into return value*/
}