#include "driver.h"

#include <stdlib.h>
#include <math.h>
static void * diskStart = NULL;

bool initDisk(){
	if(diskStart != NULL){
		return false;
	}
	diskStart = malloc(MAX_SECTOR * SECTOR_SIZE);
	return diskStart != NULL;
}
void readDisk(int fromsector, int tosector, void * toMemory){
	bool circular = false;
	if(tosector < fromsector){
		circular = true;
	}
	int dataSize = abs(tosector- fromsector)* SECTOR_SIZE;
	void * startSector = (diskStart+fromsector*SECTOR_SIZE);
	int splitDataSize = 0;
	if(circular){
		splitDataSize = tosector*SECTOR_SIZE;
		memcpy (toMemory, startSector, DISK_END);
		memcpy (toMemory, diskStart, splitDataSize);
	}else{
		memcpy (toMemory, startSector, dataSize);
 	}
}
void writeDisk(int fromsector, int tosector, const void * fromMemory){
	bool circular = false;
	if(tosector < fromsector){
		circular = true;
	}
	int dataSize = abs(tosector- fromsector)* SECTOR_SIZE;
	void * startSector = (diskStart+fromsector*SECTOR_SIZE);
	int splitDataSize = 0;
	if(circular){
		splitDataSize = tosector*SECTOR_SIZE;
		memcpy (startSector, fromMemory , DISK_END);
		memcpy (diskStart, fromMemory , splitDataSize);
	}else{
 		memcpy (startSector, fromMemory, dataSize);
 	}
}