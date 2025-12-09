#include "filesystem.h"

// --- JOSH'S IMPLEMENTATION ---

// Helper for createFile - binary search to find insertion index
static int findInsertionIndex(Directory* directory, char* name){
    int left = 0;
    int right = directory->fileCount - 1;

    while(left <= right){
        int mid = left + (right - left) / 2;
        int cmp = strcmp(directory->files[mid].fileName, name);

        if(cmp == 0){
            return mid; 
        }
        else if(cmp < 0){
            left = mid + 1;
        }
        else{
            right = mid - 1;
        }
    }

    return left;
}

void createFile(char* name, int size){
    // Check if we have space in directory
    if (dir.fileCount >= MAX_FILES) {
        printf("Error: Directory is full!\n");
        return;
    }
    
    // Check if file already exists
    if(searchDirectory(name) != -1){
        printf("Error: File already exists!\n");
        return;
    }
    
    // Check if we have enough contiguous blocks
    int start = -1;
    int consecutiveFree = 0;
    
    for (int i = 0; i < DISK_SIZE; i++) {
        if (freeBlockMap[i] == 0) {
            if (start == -1) {
                start = i;
            }
            consecutiveFree++;
            
            if (consecutiveFree >= size) {
                break;
            }
        } else {
            start = -1;
            consecutiveFree = 0;
        }
    }
    
    if (consecutiveFree < size) {
        printf("Error: Not enough contiguous free blocks available!\n");
        return;
    }

    // Mark blocks as occupied
    for(int i = start; i < start + size; i++){
        freeBlockMap[i] = 1; 
    }

    // Find where to insert the file (keeping sorted order)
    int insertionIndex = findInsertionIndex(&dir, name);

    // Shift files to make room
    for(int i = dir.fileCount; i > insertionIndex; i--){
        dir.files[i] = dir.files[i - 1];
    }

    // Create the new file entry
    FCB* newFile = &dir.files[insertionIndex];
    strcpy(newFile->fileName, name);
    newFile->fileID = insertionIndex;
    newFile->size = size * BLOCK_SIZE;
    newFile->startBlock = start;    
    newFile->numBlocks = size;
    dir.fileCount++;

    printf("File '%s' created successfully with %d blocks!\n", name, size);
}

void deleteFile(char* name) {
    int index = searchDirectory(name);

    if(index == -1){
        printf("Error: File not found!\n");
        return;
    }
    
    // Check if file is currently open
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (openFileTable[i].isOpen && openFileTable[i].fileID == index) {
            printf("Error: Cannot delete file '%s' - it is currently open!\n", name);
            return;
        }
    }

    FCB* fileToDelete = &dir.files[index];

    // Free the blocks
    for (int i = fileToDelete->startBlock; i < fileToDelete->startBlock + fileToDelete->numBlocks; i++){
        freeBlockMap[i] = 0; 
    }

    // Shift files to remove the deleted entry
    for(int i = index; i < dir.fileCount - 1; i++){
        dir.files[i] = dir.files[i + 1];
    }

    dir.fileCount--;

    printf("File '%s' deleted successfully!\n", name);
}

int searchDirectory(char* name) {
    // Binary search to find file by name
    int left = 0;
    int right = dir.fileCount - 1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        int cmp = strcmp(dir.files[mid].fileName, name);
        
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