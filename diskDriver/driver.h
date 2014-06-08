#ifndef __DRIVER_H
#define __DRIVER_H

#include <stdbool.h>

#define MAX_SECTOR 10
#define SECTOR_SIZE sizeof(block)
#define DISK_END ((MAX_SECTOR-1)*SECTOR_SIZE)

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