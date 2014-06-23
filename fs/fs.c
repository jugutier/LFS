#include "fs.h"
#include "../diskDriver/driver.h"
#include "../external_string_lib.h"

/*typedef struct
{
	it supports just one dataBlock for the moment
	void * dataBlock;
}iNode;*/

/*typedef struct
{
	int iNodeNumber;
	void * diskAddress;
}iMapEntry;*/

//this will be the cache, cleared with an external thread uppon fillup
static Block extent [EXTENT_BLOCKS];
static int currentExtentBlock = 0;
static CR * cr;
//used for the circular buffer:
static int currentDiskStart = 0;
static int currentDiskEnd = MAX_SECTOR;
static int iNodeNumber = HOME_DIRECTORY+1;//should always have an available inodenumber
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
	writeFirstSector(cr);
	return true;
}
/*
* First search for block in extent.
* If it isn't there, then check the disk.
*/
Block * getBlockByInode(int iNodeNumber)
{
	Block * b;
	int extentBlock = -1,diskBlock = -1 ;
	extentBlock = getBlockFromExtent(iNodeNumber);
	if(extentBlock != -1){
		printf("from extent\n");
		return extent+extentBlock*BLOCK_SIZE;
	}
	diskBlock = cr->imap[iNodeNumber];
	if(diskBlock != -1 ){
		printf("from disc\n");
		b = malloc(BLOCK_SIZE);
		readDisk(diskBlock, diskBlock +1 , b);
		return b;
	} 

   return NULL;
 }
//TODO: search the last occurence ??
int getBlockFromExtent(int inode){
	int i,found = 0,where = 0;
	for(i = 0;i< EXTENT_BLOCKS && !found;i++){
		if(extent[i].iNodeNumber == inode){
			found = true;
			where = i;
		}
	}
	return where;
}


Block * getBlock(const char * pathname)
{
  Block * currentBlock = getBlockByInode(HOME_DIRECTORY);
  printf("b = %p isDirectory = %d\n", currentBlock,currentBlock->isDirectory);
  Directory subdirectories[MAX_DIRECTORY];

  const char del = '/';
  char str[MAX_FILENAME_SIZE];
  strcpy(str, pathname);

  char * token, * saved;

  token = e_strtok_r(str, &del, &saved);
  int inodeNumber;

  while( token != NULL )
  {
      //if (!block->isDirectory) { return NULL; } // error
      printf("directories\n");
      getDirectories(currentBlock,&subdirectories);// returns an array of directories
      printf("iNodeNumber\n");
      inodeNumber = getInodeNumber(subdirectories, token); // get inode number for a directory name, stored in token var.
      printf("file = %s iNodeNumber %d for currentBlock %p\n",token,iNodeNumber ,currentBlock );
      currentBlock = getBlockByInode(inodeNumber);
      
      if (currentBlock == NULL) { return NULL; } // error

      token = strtok_r(NULL, &del, &saved);
  }
  return currentBlock;
}


int getInodeNumber(Directory *subdirectories, const char * token) {
  int i;
  for (i = 0; i < MAX_DIRECTORY; i++) {
    if (strcmp(subdirectories[i].fileName, token) == 0) {
      return subdirectories[i].iNodeLocation;
    }
  }
  return -1;
}

int getFilename(const char * path, char * filename)
{
    const char del = '/';
    char str[MAX_FILENAME_SIZE];
    strcpy(str, path);
    char * current,* token, * saved;

    current = e_strtok_r(str, &del, &saved);
    token  = current;
     while( token != NULL )
     {
        token = strtok_r(NULL, &del, &saved); 
        if(token != NULL){
        	current = token;
        }       
     }
	strcpy(filename, current);
    return 0;
}

int insertInFdTable(Block * block) {
	saveToExtent(block);
	return currentExtentBlock;
}

int open(char * pathname, int flags){
	int fd ,retVal;
	Block * block , * parentBlock ,b;
	char parentDirName[MAX_FILENAME_SIZE];
	switch(flags){
		case  O_RDWR:
			block = getBlock(pathname);
			 fd = insertInFdTable(block);
			return fd;
		break;
		case O_RDWR|O_CREAT:// file does not exist
			getParentDirectoryName(pathname,parentDirName);
			printf("getting parent block for %s\n",parentDirName );
			parentBlock = getBlock(parentDirName);//getParentBlock(pathname);
			char filename[MAX_FILENAME_SIZE];
      		getFilename(pathname, filename);
      		printf("parentBlock = %p filename = %s\n",parentBlock,filename );
			if((retVal = addFile(parentBlock, &b, filename))==-1){
				return -1; //error
			}
			fd = insertInFdTable(&b);
			return fd;
		break;
		case O_RDWR|O_TRUNC:

		break;
	}
}
int addFile(Block * parent,Block * newBlock,char * fileName){
	Block * fileBlock = createFileBlock();
	int iNodeForChild = addSubdirectory(parent, fileName);
	fileBlock->iNodeNumber = iNodeForChild;
	*newBlock = *fileBlock;
	return iNodeForChild;
}
/*
* Get the extent at position fildes.
* Copy nbyte bytes from buf to block->data
* If before writing
* strlen(block->data) + nbyte > MAX_DATA
* return -1.
*/
int write(int fildes, const void *buf, int nbyte){
	Block * b = &extent[fildes];
	if(strlen(b->data) + nbyte >MAX_DATA){
		return -1;
	}
	memcpy(b->data,buf,nbyte);
}

int close(int fildes) {
	/*
	* Won't do anything.
	* In next release: invalidate fildes
	*/
	return 0;
}
void initHomeDirectory(){
	Block * b =createEmptyBlock();
	b->iNodeNumber = HOME_DIRECTORY;
	saveToExtent(b);
}
Block * createEmptyBlock(){
	Block * b = malloc(sizeof(Block));
	b->isDirectory= true;
	Directory * subdirectories;
	addEmptyDirectories(&subdirectories);
	setDirectories(b,subdirectories);
	return b;
}
Block * createFileBlock(){
	Block * b = malloc(sizeof(Block));
	b->iNodeNumber = -1;
	b->isDirectory= false;
	return b;
}
void addEmptyDirectories(Directory ** listofDirectories){
	int i;
	*listofDirectories = malloc(MAX_DIRECTORY*sizeof(Directory));
	for( i = 0; i<MAX_DIRECTORY;i++){
		(*listofDirectories)[i].iNodeLocation = -1;
	}

}
/*
	* go to parent directory and add another directory into
	* list of directories. create a new block with
	* subdiretories for the new "folder". addToExtent child.
	*/
int makeDirectory(const char * dirName){
	printf("makeDirectory: %s\n",dirName );
	char fileName[MAX_FILENAME_SIZE];
	getFilename(dirName, fileName);
	printf("filename =%s\n",fileName);
	char parentDir[MAX_FILENAME_SIZE];
	getParentDirectoryName(dirName, parentDir);
	printf("parentDir =%s\n",parentDir);
	Block * parentBlock = getBlock(parentDir);
	Block * childBlock;
	int iNodeForChild;

	if ((iNodeForChild = addSubdirectory(parentBlock, fileName))!=-1) {
		//saveToExtent(parentBlock);
		childBlock = createEmptyBlock();
		childBlock->iNodeNumber = iNodeForChild;
		saveToExtent(childBlock);
		printf("directory complete\n");
		return 0;
	}
	return -1;
}
int addSubdirectory(Block *parentBlock, char * dirName){
	Directory listofDirectories[MAX_DIRECTORY];
	getDirectories(parentBlock,&listofDirectories);
	int i;
	for(i=0;i<MAX_DIRECTORY;i++){
		if (listofDirectories[i].iNodeLocation==-1)
		{
			listofDirectories[i].iNodeLocation = iNodeNumber;
			iNodeNumber++;
			strcpy(listofDirectories[i].fileName,dirName);
			break;
		}
	}
	setDirectories(parentBlock,listofDirectories);
	if(i == MAX_DIRECTORY){
		return -1;//no more directories allowed
	}else{
		return iNodeNumber - 1;
	}
}
void getParentDirectoryName(const char * path, char * parentDirName) {
	strcpy(parentDirName, path);
	char filename[MAX_FILENAME_SIZE];
	getFilename(path, filename);
	int filenameLength = strlen(filename);
	int dirNameLength = strlen(parentDirName);
	parentDirName[dirNameLength - filenameLength] = '\0';
}

void saveToExtent(Block * block){
	/**
	*check if im running out of space and clear if needed
	**/
	if(currentExtentBlock == floor(EXTENT_BLOCKS*0.9)){
		printf("extentToDisk\n");
		extentToDisk();
	}
	printf("saved %p in currentExtentBlock = %d\n",/*(extent+currentExtentBlock*BLOCK_SIZE),*/&extent[currentExtentBlock],currentExtentBlock);
	memcpy(&extent[currentExtentBlock] , block,BLOCK_SIZE) ;
	currentExtentBlock++;
}
//persists extent to disk, and clears it;
void extentToDisk(){
	//updateCR for each block
	int i;
	for ( i = 0; i < EXTENT_BLOCKS; ++i)
	{
		if(extent[i].iNodeNumber == -1){
			extent[i].iNodeNumber = iNodeNumber;
			iNodeNumber++;
		}
		writeDisk(currentDiskEnd+i, currentDiskEnd+i+1, (void *)&extent[i]/*extent+i*BLOCK_SIZE*/);
		memset(&extent[i]/*(void *)extent+i*BLOCK_SIZE*/,0,BLOCK_SIZE);
	}
	currentExtentBlock = 0;
}
//return list of strings with names or null if not a directory
/*getBlock of that path,if its not a diretory return null,
 save list subdirectoires into return value*/
char** list(char * path){
	printf("list: %s\n",path);
	Block * b;int i;
	Directory listofDirectories[MAX_DIRECTORY];
	char  **retVal;
	b = getBlock(path);
	//printf("b = %p\n",b );
	if(b == NULL || !b->isDirectory){
		return NULL;
	}
	getDirectories(b,&listofDirectories);
	retVal = malloc(MAX_DIRECTORY);
	for(i=0;i<MAX_DIRECTORY;i++){		
		if(listofDirectories[i].fileName[0]!='\0'){
			retVal[i] = malloc(MAX_FILENAME_SIZE*sizeof(char));
			strcpy(retVal[i],listofDirectories[i].fileName);
		}else{
			break;
		}
		
	}
	return retVal;
}
/*
*Deserialize directories from b into d
*/
void getDirectories(Block * b, Directory ** d){
	memcpy(d,b->data,MAX_DIRECTORY*sizeof(Directory));
}
void setDirectories(Block *b, Directory * d){
	memcpy(b->data, (char*)d, MAX_DATA);
}