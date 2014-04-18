#include "key.h"

#include <string.h>
#include <sys/stat.h>

//return byte from key at offset "position"
char getBit(xorKey *skey, int position) {
   return skey->myKey[position%skey->myKeyLength];
} 

//utility function to generate a key of length "length"
void genKey( xorKey *pkey,int length) {
  pkey->myKeyLength=length;
  pkey->myKey=(char*)malloc(sizeof(char)*length);
  int i=0;
  for(i=0;i<length;i++) {
     pkey->myKey[i]=(char)(rand()%cursysCharLen);
  } 
}

// frees the memory allocated to support a key
void freeKey(xorKey* key) {
    if(key->myKey != NULL) {
        free(key->myKey);
        key->myKey = NULL;
    }
}

// reads in a key from a file
void readKey(xorKey* key, const char* filename) {
    // Sets the initial key metadata
    key->myKey = NULL;
    key->myKeyLength = 0;
    if(strlen(filename)<MAXDESCLENGTH) {
        strcpy(key->myKeyDescriptor,filename);
    } else {
        strncpy(key->myKeyDescriptor,filename+strlen(filename)-(MAXDESCLENGTH+1),MAXDESCLENGTH);
    }

    // Reads in the key data
    off_t keyLen = fsize(filename);
    key->myKeyLength = keyLen;
    key->myKey = (char*)malloc(keyLen);
    FILE* kfile = (FILE*)fopen(filename,"rb");
    if(kfile==NULL) {
        printf("Can't open %s\n",filename);
        exit(EXIT_FAILURE);
    }
    fread(key->myKey, keyLen, 1, kfile);
    fclose(kfile);
}

// utility function to get the length of a file -- very important since
//     some actual data bytes may be 0 (the null char indicating the end
//     of a string.
// generously donated by Stackoverflow: http://stackoverflow.com/questions/8236/how-do-you-determine-the-size-of-a-file-in-c
off_t fsize(const char *filename) {
    struct stat st; 
    if (stat(filename, &st) == 0) {
        return st.st_size;
    }
    return -1; 
}
