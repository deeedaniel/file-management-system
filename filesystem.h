#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// --- Constants ---
#define DISK_SIZE 100
#define BLOCK_SIZE 512
#define MAX_FILES 50
#define MAX_OPEN_FILES 10

// --- Structures ---

// ZORAIZ: You will refine this structure
typedef struct {
    char fileName[32];
    int size;
    int startBlock;
    int numBlocks;
    int fileID;
} FCB;

// JOSH: You will refine this structure
typedef struct {
    FCB files[MAX_FILES];
    int fileCount;
} Directory;

// DANIEL: This is your structure
typedef struct {
    int fileID;
    int currentPointer;
    bool isOpen;
} OpenFileEntry;

// --- Global Variables (To be defined in main.c or respective files) ---
extern char virtualDisk[DISK_SIZE][BLOCK_SIZE];
extern int freeBlockMap[DISK_SIZE];
extern Directory dir;
extern OpenFileEntry openFileTable[MAX_OPEN_FILES];

// --- Function Prototypes ---

// ZORAIZ'S TASKS (storage.c)
void initSystem();
int findFreeBlock();

// JOSH'S TASKS (directory.c)
void createFile(char* name, int size);
void deleteFile(char* name);
int searchDirectory(char* name);

// DANIEL'S TASKS (main.c)
void openFile(char* name);
void closeFile(char* name);

#endif