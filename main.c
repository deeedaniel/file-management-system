#include "filesystem.h"

// Define the global variables here
char virtualDisk[DISK_SIZE][BLOCK_SIZE];
int freeBlockMap[DISK_SIZE];
Directory dir;
OpenFileEntry openFileTable[MAX_OPEN_FILES];

// --- DANIEL'S IMPLEMENTATION ---
void openFile(char* name) {
    int index = searchDirectory(name);
    if (index == -1) {
        printf("Error: File not found.\n");
        return;
    }

    // Check if file is already open
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (openFileTable[i].isOpen && openFileTable[i].fileID == index) {
            printf("Error: File '%s' is already open.\n", name);
            return;
        }
    }

    // Find a free slot in the Open File Table
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (!openFileTable[i].isOpen) {
            openFileTable[i].fileID = index;
            openFileTable[i].currentPointer = 0;
            openFileTable[i].isOpen = true;
            printf("File %s opened successfully.\n", name);
            return;
        }
    }

    printf("Error: Maximum number of open files reached.\n");
}

void closeFile(char* name) {
    int index = searchDirectory(name);
    if (index == -1) {
        printf("Error: File not found.\n");
        return;
    }

    // Find the file in the Open File Table
    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        if (openFileTable[i].isOpen && openFileTable[i].fileID == index) {
            openFileTable[i].isOpen = false;
            printf("File %s closed successfully.\n", name);
            return;
        }
    }

    printf("Error: File '%s' is not currently open.\n", name);
}

void writeFile(char* name, const char* data, int length) {
    int  fileID = searchDirectory(name);

    if(fileID == -1) {
        printf("Error: File '%s' not found.\n", name);
        return;
    }

    OpenFileEntry* openEntry = NULL;

    for(int i = 0; i < MAX_OPEN_FILES; i++) {
        if(openFileTable[i].isOpen && openFileTable[i].fileID == fileID) {
            openEntry = &openFileTable[i];
            break;
        }
    }

    if(openEntry == NULL) {
        printf("Error: File '%s' is not open.\n", name);
        return;
    }

    FCB* file = &dir.files[fileID];

    int remaining = file->size - openEntry->currentPointer;
    if(remaining <= 0) {
        printf("Error: No space left in file '%s'.\n", name);
        return;
    }

    int toWrite;

    if(length > remaining) {
        toWrite = remaining;
    } else {
        toWrite = length;
    }

    int absolutePosition = openEntry->currentPointer;
    int blockIndex = absolutePosition / BLOCK_SIZE;
    int blockOffset = absolutePosition % BLOCK_SIZE;
    int diskBlock = file->startBlock + blockIndex;
    int bytesWritten = 0;

    while(bytesWritten < toWrite) {
        virtualDisk[diskBlock][blockOffset] = data[bytesWritten];

        bytesWritten++;
        blockOffset++;

        if(blockOffset == BLOCK_SIZE) {
            blockOffset = 0;
            diskBlock++;

            if(diskBlock >= file->startBlock + file->numBlocks) {
                break;
            }
        }
    }

    openEntry->currentPointer += bytesWritten;
    printf("Wrote %d bytes to file '%s'.\n", bytesWritten, name);
}

int main() {
    initSystem(); // Calls Zoraiz's function
    
    int choice;
    char nameBuffer[32];
    int sizeBuffer;

    printf("--- File Management System Initialized ---\n");

    while(1) {
        printf("\n1. Create File\n2. Delete File\n3. Open File\n4. Close File\n5. Exit\n");
        printf("Select option: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: // Josh's Task
                printf("Enter filename: ");
                scanf("%s", nameBuffer);
                printf("Enter size: ");
                scanf("%d", &sizeBuffer);
                createFile(nameBuffer, sizeBuffer);
                break;

            case 2: // Josh's Task
                printf("Enter filename: ");
                scanf("%s", nameBuffer);
                deleteFile(nameBuffer);
                break;

            case 3: // Daniel's Task
                printf("Enter filename: ");
                scanf("%s", nameBuffer);
                openFile(nameBuffer);
                break;
            
            case 4: // Daniel's Task
                printf("Enter filename: ");
                scanf("%s", nameBuffer);
                closeFile(nameBuffer);
                break;

            case 5:
                printf("Exiting system.\n");
                return 0;

            default:
                printf("Invalid option.\n");
        }
    }
    return 0;
}