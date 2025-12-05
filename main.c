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
    // Logic to add to openFileTable goes here...
    printf("File %s opened.\n", name);
}

void closeFile(char* name) {
    // Logic to remove from openFileTable goes here...
    printf("File %s closed.\n", name);
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