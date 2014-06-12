#ifndef __FS_H
#define __FS_H
#include <stdbool.h>

#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_CREAT		0x0200		/* create if nonexistant */
#define	O_TRUNC		0x0400		/* truncate to zero length */

#define BLOCK_SIZE sizeof(block)

bool initFS();

/*
*open
*pathname eg '/pepe/a.txt' always full path
* flags O_RDWR alone if file exists or '|' with O_CREAT or O_TRUNC
*
*return filedescriptor to pathname
*/
int open(char * pathname, int flags);

int write(int fildes, const void *buf, int nbyte);

void makedirectory(char * directoryName);

bool cd(char * dirName);

void ls();
#endif