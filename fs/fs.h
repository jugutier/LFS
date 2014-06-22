#ifndef __FS_H
#define __FS_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_CREAT		0x0200		/* create if nonexistant */
#define	O_TRUNC		0x0400		/* truncate to zero length */

#define HOME_DIRECTORY 1//inode number for '/' the home directory
#define MAX_DATA 248
#define EXTENT_BLOCKS 5
#define MAX_DIRECTORY 13
#define MAX_FILENAME_SIZE 12

typedef struct
{
	int iNodeNumber;
	bool isDirectory;
	char data[MAX_DATA];//text file or Directory as byte string
}Block;//sizeof = 256

#define BLOCK_SIZE sizeof(Block)

bool initFS();

/*
*open
*pathname eg '/pepe/a.txt' always full path
* flags O_RDWR alone if file exists or '|' with O_CREAT or O_TRUNC
*
*return filedescriptor to pathname
*/
int open(char * pathname, int flags);

/**
* read from file fildes into buff (allocated by user) nbytes
* return the actual number of bytes read.
*/
int read(int fildes, void * buff,int nbyte);

int write(int fildes, const void *buf, int nbyte);
/*
*returns 0 if succesfull
*/
int close(int fildes);

void makedirectory(char * directoryName);

char ** list(char * path);


int getFilename(const char * path, char * filename);
#endif
