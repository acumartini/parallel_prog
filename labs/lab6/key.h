#ifndef _KEY_H
#define _KEY_H
/*
 * Defines some utility functions for working with keys
 */

#define MAXKEYLENGTH 65536
#define MAXDESCLENGTH 128
#define cursysCharLen (2<<(sizeof(char)*8))

#ifndef true
#include <stdlib.h>
#include <stdio.h>
#define true 1
#define false 0
#endif

#include <sys/types.h>

// In memory key representation
typedef struct 
{
   int myKeyLength; // length of the key
   char myKeyDescriptor[MAXDESCLENGTH]; // field for key metadata, if any
                                        // not necessarily null terminated
   char* myKey; // The key data segment
} xorKey;

// frees the memory allocated when creating a key
extern void freeKey(xorKey* key);

// reads in a key from a file
extern void readKey(xorKey* key, const char* filename);

//return byte from key at offset "position"
extern char getBit(xorKey *skey, int position);

//utility function to generate a key of length "length"
extern void genKey( xorKey *pkey,int length);

// utility function to get the length of a file -- very important since
//     some actual data bytes may be 0 (the null char indicating the end
//     of a string.
extern off_t fsize(const char *filename);

#endif
