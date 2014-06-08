#ifndef __FS_H
#define __FS_H
#include <stdbool.h>

#define	O_RDWR		0x0002		/* open for reading and writing */
#define	O_CREAT		0x0200		/* create if nonexistant */
#define	O_TRUNC		0x0400		/* truncate to zero length */

bool initFS();

/*
*open
*pathname eg '/pepe/a.txt'
* flags O_RDWR alone if file exists or '|' with O_CREAT or O_TRUNC
*
*return filedescriptor to pathname
*/
int open(char * pathname, int flags);

int write(int fildes, const void *buf, int nbyte);

void makedirectory(char * directoryName);

void nano(char * filename,char * text);

bool cd(char * dirName);

void ls();
#endif