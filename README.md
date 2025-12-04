# File Management System

This is the repository for our group project. We are simulating a Single-Level Directory File Management System in C.

## Compilation

To compile the project, run the following command in the terminal:

gcc main.c storage.c directory.c -o fms
./fms

## File Structure

Please only edit the file assigned to you to avoid merge conflicts.

- filesystem.h: Shared header file with struct definitions. Do not change this without telling the group.
- main.c: Daniel's file. Handles the interface and open file table.
- storage.c: Zoraiz's file. Handles the disk simulation.
- directory.c: Josh's file. Handles file logic.

## Task Assignments

### Zoraiz

File: storage.c

- Implement initSystem(): Zero out the virtualDisk array.
- Implement findFreeBlock(): Find an empty index in the disk.
- Report: Research APFS vs NTFS vs FAT32.

### Josh

File: directory.c

- Implement createFile(): Logic to add file to directory.
- Implement deleteFile(): Logic to remove file and clear blocks.
- Implement searchDirectory(): Return index of a file.
- Report: Research on file copying and metadata loss.

### Daniel

File: main.c

- Implement openFile(): Add file to Open File Table.
- Implement closeFile(): Remove file from Open File Table.
- Implement main(): The switch statement and menu loop.
- Report: Abstract, Introduction, and Conclusion.
