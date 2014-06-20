#include "fs.h"
#include "../diskDriver/driver.h"
#include "../external_string_lib.h"

/*typedef struct
{
	it supports just one dataBlock for the moment
	void * dataBlock;
}iNode;*/

typedef struct{//use N of this in block data
	char fileName [12];
	int iNodeLocation;
}Directory;//sizeof 16 * (N=16) = 256

/*typedef struct
{
	int iNodeNumber;
	void * diskAddress;
}iMapEntry;*/

typedef struct{
	int imap [64];//key inodenumber,  disk sector positions
}CR;//sizeof = 256

//this will be the cache, cleared with an external thread uppon fillup
static Block extent [EXTENT_BLOCKS];
static int currentExtentBlock = 0;
static CR * cr;
//used for the circular buffer:
static int currentDiskStart = 0;
static int currentDiskEnd = MAX_SECTOR;

/************PRIVATE FUNCTIONS **********/
bool saveCR();
void initHomeDirectory();
Block * getBlock(char * pathname);
void saveToExtent(Block * block);
void extentToDisk();
char** list(char * path);
void addEmptyDirectories(Directory ** listofDirectories);
/************PRIVATE FUNCTIONS **********/

bool initFS(){
	if(cr == NULL){
		cr = malloc(sizeof(CR));
		if(cr == NULL)
			return false;
	}
	saveCR();
	initHomeDirectory();
	return true;
}

bool saveCR(){
	writeDisk(currentDiskStart, currentDiskStart+1, cr);
	return true;
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

int
getFilename(const char * path, char * filename)
{
    const char del = '/';
    char str[MAX_FILENAME_SIZE];
    strcpy(str, path);

    char * token, * saved;

    token = e_strtok_r(str, &del, &saved);

     while( token != NULL )
     {
        token = strtok_r(NULL, &del, &saved);
        if (token != NULL) strcpy(filename, token);
     }

    return 0;
}

int insertInFdTable(Block * block) {

	saveToExtent(block);
	return currentExtentBlock;
}

int open(char * pathname, int flags){
	int fd;
	Block * block , * parentBlock ,b;
	switch(flags){
		case  O_RDWR:
			block = getBlock(pathname);
			 fd = insertInFdTable(block);
			return fd;
		break;
		case O_RDWR|O_CREAT:
				// file does not exist
			parentBlock = getBlock(pathname);//getParentBlock(pathname);
			char filename[100];
      getFilename(pathname, filename);
			//malloc b??
			b.isDirectory = false;

			//TODO:addFile(parentBlock, &b, filename); // if (... == true)
			fd = insertInFdTable(&b);
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
	Block * b = malloc(sizeof(Block));
	b->isDirectory= true;
	Directory** subdirectories;
	addEmptyDirectories(subdirectories);
	memcpy(b->data,subdirectories, MAX_DATA);
	saveToExtent(b);
}

void addEmptyDirectories(Directory ** listofDirectories){
	Directory newDirectories [MAX_DIRECTORY];//malloc
	//listofDirectories = &newDirectories; asign what maloc returns
	int i;
	for( i = 0; i<MAX_DIRECTORY;i++){
		newDirectories[i].iNodeLocation = -1;
	}
}

void makedirectory(char * dirName){
	/*
	* go to parent directory and add another directory into
	* list of directories. log parent.create a new block with
	* subdiretories for the new "folder". log child.
	*/
}
void saveToExtent(Block * block){
	/**
	*check if im running out of space and clear if needed
	**/
	if(currentExtentBlock == floor(EXTENT_BLOCKS*0.9)){
		extentToDisk();
	}
	memcpy((extent+currentExtentBlock*BLOCK_SIZE) , block,BLOCK_SIZE) ;
	currentExtentBlock++;
}
//persists extent to disk, and clears it;
void extentToDisk(){
	//updateCR for each block
	int i;
	for ( i = 0; i < EXTENT_BLOCKS; ++i)
	{
		writeDisk(currentDiskEnd+i, currentDiskEnd+i+1, (void *)extent+i*BLOCK_SIZE);
		memset((void *)extent+i*BLOCK_SIZE,0,BLOCK_SIZE);
	}
	currentExtentBlock = 0;
}
//return list of strings with names or null if not a directory
char** list(char * path){
	/*getBlock of that path,if its not a diretory return null,
	 save list subdirectoires into return value*/
	return NULL;
}
