#include "driver.h"

#include <stdlib.h>
#include <math.h>
static void * diskStart = NULL;

bool initDisk(){
	if(diskStart != NULL){
		return false;
	}
	diskStart = malloc(SECTOR_QTY * SECTOR_SIZE);
	return diskStart != NULL;
}
void readDisk(int fromsector, int tosector, void * toMemory){
	bool circular = false;
	int dataSize = 0,splitDataSize = 0;
	void * startSector = (diskStart+fromsector*SECTOR_SIZE);
	if(tosector == fromsector)
		return;
	else if(tosector > fromsector){
		dataSize = (tosector - fromsector)* SECTOR_SIZE;
		memcpy (toMemory, startSector, dataSize);
	}else{
		dataSize = (MAX_SECTOR - fromsector)* SECTOR_SIZE;
		splitDataSize = tosector * SECTOR_SIZE;
		memcpy (toMemory, startSector, dataSize);
		memcpy ((toMemory + dataSize) , diskStart, splitDataSize);
	}
}
void writeDisk(int fromsector, int tosector, const void * fromMemory){
	bool circular = false;
	int dataSize = 0,splitDataSize = 0;
	void * startSector = (diskStart+fromsector*SECTOR_SIZE);
	if(tosector == fromsector)
		return;
	else if(tosector > fromsector){		
		dataSize = (tosector - fromsector)* SECTOR_SIZE;
		//printf("pincha %d - start %p - from %p \n",dataSize, startSector, fromMemory);
		memcpy (startSector, fromMemory, dataSize);
	}else{
		dataSize = (MAX_SECTOR - fromsector)* SECTOR_SIZE;
		splitDataSize = tosector*SECTOR_SIZE;
		memcpy (startSector, fromMemory , DISK_END);
		memcpy (diskStart, (fromMemory+dataSize) , splitDataSize);
	}
}