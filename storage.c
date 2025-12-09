#include "filesystem.h"

// --- ZORAIZ'S IMPLEMENTATION ---

void initSystem() {
    // Zero out the virtualDisk
    for (int i = 0; i < DISK_SIZE; i++) {
        for (int j = 0; j < BLOCK_SIZE; j++) {
            virtualDisk[i][j] = 0;
        }
    }
    
    // Initialize freeBlockMap (0 = free, 1 = occupied)
    for (int i = 0; i < DISK_SIZE; i++) {
        freeBlockMap[i] = 0;
    }
    
    // Initialize directory
    dir.fileCount = 0;
    
    // Initialize open file table
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        openFileTable[i].isOpen = false;
        openFileTable[i].fileID = -1;
        openFileTable[i].currentPointer = 0;
    }
    
    printf("System initialized successfully.\n");
}

int findFreeBlock() {
    // Find first contiguous free block
    for (int i = 0; i < DISK_SIZE; i++) {
        if (freeBlockMap[i] == 0) {
            return i;
        }
    }
    return -1; // No free blocks
}