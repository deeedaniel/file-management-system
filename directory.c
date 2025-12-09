#include "filesystem.h"

// --- JOSH'S IMPLEMENTATION ---

extern Directory directory;

void createFile(char* name, int size) {
    printf("[DEBUG] Creating file: %s (Josh to implement)\n", name);
    int start = findFreeBlock();

    if(start == -1){
        printf("No free blocks available!\n");
        return;
    }

    if(searchFile(&directory, name) != -1){
        printf("File already exists!\n");
        return;
    }

    for(int i = start; i < start + size; i++){
        freeBlockMap[i] = 1; 
    }

    int insertionIndex = findInsertionIndex(&directory, name);

    for(int i = directory.fileCount; i > insertionIndex; i--){
        directory.files[i] = directory.files[i - 1];
    }

    FCB* newFile = &directory.files[insertionIndex];
    strcpy(newFile->fileName, name);
    newFile->fileID = insertionIndex;
    newFile->size = size * BLOCK_SIZE;
    newFile->startBlock = start;    
    newFile->numBlocks = size;
    directory.fileCount++;

    printf("File '%s' created successfully!\n", name);
}

void deleteFile(char* name) {
    printf("[DEBUG] Deleting file: %s (Josh to implement)\n", name);
        int index = searchFile(&directory, name);

    if(index == -1){
        printf("File not found!\n");
        return;
    }

    FCB* fileToDelete = &directory.files[index];

    for (int i = fileToDelete->startBlock; i < fileToDelete->startBlock + fileToDelete->numBlocks; i++){
        freeBlockMap[i] = 0; 
    }

    for(int i = index; i < directory.fileCount - 1; i++){
        directory.files[i] = directory.files[i + 1];
    }

    directory.fileCount--;

    printf("File '%s' deleted successfully!\n", name);
}

int searchDirectory(char* name) {
    // Return index if found or -1 if not found
    int left = 0;
    int right = directory.fileCount - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(directory.files[mid].fileName, name);
        
        if (cmp == 0) {
            return mid; 
        } 
        else if (cmp < 0) {
            left = mid + 1;
        } 
        else {
            right = mid - 1;
        }
    }

    return -1;
}