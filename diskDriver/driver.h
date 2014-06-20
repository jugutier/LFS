#ifndef __DRIVER_H
#define __DRIVER_H

#include <stdbool.h>
#include "../fs/fs.h"//only for sizeof Block, TODO: remove

#define SECTOR_QTY 10
#define MAX_SECTOR (SECTOR_QTY-1)
#define SECTOR_SIZE sizeof(Block)
#define DISK_END (MAX_SECTOR*SECTOR_SIZE)

/**
*initDisk
*@return true if the disk was inited false if it was already 
*there or if malloc failed
**/
bool initDisk();
/**
*readDisk
*read fromsector tosector into toMemory
*@user MUST allocate necesary space in toMemory
*@warning it reads circularly
**/
void readDisk(int fromsector, int tosector, void * toMemory);
/**
*writeDisk
*writes fromMemory into fromsector upto tosector
*@warning it writes circularly
**/
void writeDisk(int fromsector, int tosector, const void * fromMemory);
#endif