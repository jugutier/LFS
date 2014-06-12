#include "fs.h"
#include "../diskDriver/driver.h"

#define HOME_DIRECTORY 1//inode number for '/' the home directory
#define MAX_DATA 260
#define EXTENT_BLOCKS 5
typedef struct 
{
	/*it supports just one dataBlock for the moment*/
	void * dataBlock;
}iNode;

typedef struct{
	/*this should be a map to support multiple files per directory*/
	char fileName [16];
	int iNodeLocation;
}Directory;//sizeof will be MAX_DATA. or 13 pairs of filename + inode

typedef struct 
{
	int iNodeNumber;
	void * diskAddress;
}iMapEntry;

typedef struct 
{
	iMapEntry entry;
	iNode inode;
	char data[MAX_DATA];//text file or Directory as byte string
}block;//sizeof = 272

typedef struct{
	iMapEntry imap [34];
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

void initHomeDirectory(){
	Directory * homeDirectory = malloc(sizeof(Directory));
	homeDirectory->fileName[0]='/';
	homeDirectory->iNodeLocation = HOME_DIRECTORY;
	//writeDisk(currentDiskStart+1, currentDiskStart+2, homeDirectory);
}

void mkdir(char * dirName){

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
	for (int i = 0; i < EXTENT_BLOCKS; ++i)
	{
		writeDisk(currentDiskEnd+i, currentDiskEnd+i+1, extent[i]);
		memset(extent[i],0,BLOCK_SIZE);
	}
	currentExtentBlock = 0;
}