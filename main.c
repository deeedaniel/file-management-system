#include "filesystem.h"

// Define the global variables here
char virtualDisk[DISK_SIZE][BLOCK_SIZE];
int freeBlockMap[DISK_SIZE];
Directory dir;
OpenFileEntry openFileTable[MAX_OPEN_FILES];
char activeFileName[32] = "";  // Track currently active file

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
            strcpy(activeFileName, name);  // Set as active file
            printf("✓ File '%s' opened and ready for reading/writing.\n", name);
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
            // Save name before clearing active file
            char savedName[32];
            strcpy(savedName, name);
            if (strcmp(activeFileName, name) == 0) {
                activeFileName[0] = '\0';  // Clear active file
            }
            printf("✓ File '%s' closed successfully.\n", savedName);
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
    printf("✓ Data written to '%s' successfully.\n", name);
}

void readFile(char* name) {
    int fileID = searchDirectory(name);
    
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
    
    // Reset to beginning for reading (simpler for this project)
    openEntry->currentPointer = 0;
    
    printf("Reading from file '%s':\n", name);
    printf("--- Content Start ---\n");
    
    int blockOffset = 0;
    int diskBlock = file->startBlock;
    int bytesRead = 0;
    bool foundData = false;
    
    // Read until we hit null terminator or end of file
    while(bytesRead < file->size) {
        char byte = virtualDisk[diskBlock][blockOffset];
        
        // Stop at null terminator
        if(byte == '\0') {
            break;
        }
        
        // Only print non-zero bytes (actual data)
        if(byte != 0) {
            printf("%c", byte);
            foundData = true;
        }
        
        bytesRead++;
        blockOffset++;
        
        if(blockOffset == BLOCK_SIZE) {
            blockOffset = 0;
            diskBlock++;
            
            if(diskBlock >= file->startBlock + file->numBlocks) {
                break;
            }
        }
    }
    
    if(!foundData) {
        printf("(File is empty)");
    }
    
    printf("\n--- Content End ---\n");
}

int main() {
    initSystem(); // Calls Zoraiz's function
    
    int choice;
    char nameBuffer[32];
    int sizeBuffer;
    char dataBuffer[1024];

    printf("--- File Management System Initialized ---\n");

    while(1) {
        printf("\n=== File Management System Menu ===\n");
        
        // Show active file status
        if (activeFileName[0] != '\0') {
            printf("[Active File: %s]\n", activeFileName);
        }
        
        printf("1. Create File\n");
        printf("2. Delete File\n");
        printf("3. Open File\n");
        printf("4. Close File\n");
        printf("5. Write to File\n");
        printf("6. Read from File\n");
        printf("7. List Files\n");
        printf("8. Exit\n");
        printf("Select option: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: // Josh's Task - Create File
                printf("Enter filename: ");
                scanf("%s", nameBuffer);
                printf("Enter size (in blocks): ");
                scanf("%d", &sizeBuffer);
                createFile(nameBuffer, sizeBuffer);
                break;

            case 2: // Josh's Task - Delete File
                printf("Enter filename: ");
                scanf("%s", nameBuffer);
                deleteFile(nameBuffer);
                break;

            case 3: // Daniel's Task - Open File
                printf("Enter filename: ");
                scanf("%s", nameBuffer);
                openFile(nameBuffer);
                break;
            
            case 4: // Daniel's Task - Close File
                if (activeFileName[0] != '\0') {
                    // Close the active file
                    closeFile(activeFileName);
                } else {
                    // No active file, ask for filename
                    printf("No file is currently open. Enter filename: ");
                    scanf("%s", nameBuffer);
                    closeFile(nameBuffer);
                }
                break;
            
            case 5: // Daniel's Task - Write to File
                if (activeFileName[0] != '\0') {
                    // Use active file
                    printf("Enter data to write: ");
                    scanf(" %[^\n]", dataBuffer); // Read entire line including spaces
                    writeFile(activeFileName, dataBuffer, strlen(dataBuffer));
                } else {
                    // No active file, ask for filename
                    printf("No file is currently open. Enter filename: ");
                    scanf("%s", nameBuffer);
                    printf("Enter data to write: ");
                    scanf(" %[^\n]", dataBuffer);
                    writeFile(nameBuffer, dataBuffer, strlen(dataBuffer));
                }
                break;
            
            case 6: // Daniel's Task - Read from File
                if (activeFileName[0] != '\0') {
                    // Use active file
                    readFile(activeFileName);
                } else {
                    // No active file, ask for filename
                    printf("No file is currently open. Enter filename: ");
                    scanf("%s", nameBuffer);
                    readFile(nameBuffer);
                }
                break;
            
            case 7: // List all files
                printf("\n=== Directory Listing ===\n");
                if (dir.fileCount == 0) {
                    printf("No files in directory.\n");
                } else {
                    printf("%-20s %-10s %-12s %-10s\n", "Filename", "Size (B)", "Start Block", "# Blocks");
                    printf("-----------------------------------------------------------\n");
                    for (int i = 0; i < dir.fileCount; i++) {
                        printf("%-20s %-10d %-12d %-10d\n", 
                               dir.files[i].fileName,
                               dir.files[i].size,
                               dir.files[i].startBlock,
                               dir.files[i].numBlocks);
                    }
                }
                break;

            case 8:
                printf("Exiting system.\n");
                return 0;

            default:
                printf("Invalid option.\n");
        }
    }
    return 0;
}