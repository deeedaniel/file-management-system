#include "filesystem.h"

// --- JOSH'S IMPLEMENTATION ---

void createFile(char* name, int size) {
    printf("[DEBUG] Creating file: %s (Josh to implement)\n", name);
    // 1. Check if name exists
    // 2. Call findFreeBlock()
    // 3. Update Directory
}

void deleteFile(char* name) {
    printf("[DEBUG] Deleting file: %s (Josh to implement)\n", name);
    // 1. Find file
    // 2. Clear blocks
    // 3. Remove from Directory
}

int searchDirectory(char* name) {
    // Return index of file in directory array
    return -1; // Placeholder
}