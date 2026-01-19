#include "header.h"


void list_root_dir(void *ptr, struct superblock *sb) {
    // 1. Find the Inode table start
    struct dinode *dip = (struct dinode *)(ptr + (sb->inodestart * BSIZE));
    
    // 2. Root directory is always Inode #1 [cite: 48]
    struct dinode *root_inode = &dip[1];

    // 3. Look at the data blocks of the root directory
    // For simplicity, we assume the root dir fits in the first direct block (addrs[0])
    struct dirent *de = (struct dirent *)(ptr + (root_inode->addrs[0] * BSIZE));

    // 4. Iterate through directory entries
    for (int i = 0; i < BSIZE / sizeof(struct dirent); i++) {
        if (de[i].inum != 0) { // If the entry is not empty
            // Get the specific inode for this file to find its type and size
            struct dinode *file_inode = &dip[de[i].inum];
            
            // Print: Name Type Inode Size 
            printf("%-14s %d %d %d\n", de[i].name, file_inode->type, de[i].inum, file_inode->size);
        }
    }
}

void copy_file(void *ptr, struct superblock *sb, char *xv6_name, char *linux_name) {
    // 1. Locate the Inode table and the Root Inode (Inode #1) [cite: 50, 62]
    struct dinode *dip = (struct dinode *)(ptr + (sb->inodestart * BSIZE));
    struct dinode *root_inode = &dip[1];

    // 2. Search for the file in the root directory [cite: 59, 60]
    struct dirent *de = (struct dirent *)(ptr + (root_inode->addrs[0] * BSIZE));
    int found_inum = 0;

    for (int i = 0; i < BSIZE / sizeof(struct dirent); i++) {
        if (de[i].inum != 0 && strcmp(de[i].name, xv6_name) == 0) {
            found_inum = de[i].inum;
            break;
        }
    }

    // 3. Error handling if file doesn't exist [cite: 60]
    if (found_inum == 0) {
        fprintf(stderr, "File %s does not exist in the root directory\n", xv6_name);
        exit(1);
    }

    // 4. Get the file's inode and open the Linux destination file [cite: 55, 58]
    struct dinode *file_inode = &dip[found_inum];
    int out_fd = open(linux_name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (out_fd < 0) {
        perror("linux open");
        exit(1);
    }

    // 5. Read data blocks and write to Linux [cite: 50, 62]
    uint bytes_remaining = file_inode->size;
    
    // Direct blocks (first 12) 
    for (int i = 0; i < NDIRECT && bytes_remaining > 0; i++) {
        uint n = (bytes_remaining > BSIZE) ? BSIZE : bytes_remaining;
        write(out_fd, ptr + (file_inode->addrs[i] * BSIZE), n);
        bytes_remaining -= n;
    }

    // Indirect block (the 13th address) 
    if (bytes_remaining > 0) {
        uint *indirect_block = (uint *)(ptr + (file_inode->addrs[NDIRECT] * BSIZE));
        for (int i = 0; i < NINDIRECT && bytes_remaining > 0; i++) {
            uint n = (bytes_remaining > BSIZE) ? BSIZE : bytes_remaining;
            write(out_fd, ptr + (indirect_block[i] * BSIZE), n);
            bytes_remaining -= n;
        }
    }

    close(out_fd);
}