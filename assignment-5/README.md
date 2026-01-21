# xv6 Filesystem Reader

## Functionality

This project is a user-space utility that reads a raw xv6 filesystem image (`fs.img`) without requiring you to mount it or boot up QEMU. It parses the binary structure of the disk (superblock, inodes, bitmaps, and data blocks) to perform these basic operations.

It currently supports two main commands:
1. **ls**: Lists files in the root directory, along with their type (file/dir), inode number, and size. 
2. **cp**: Extracts a file from inside the filesystem image and saves it to your local machine. It handles direct and indirect blocks, allowing for the extraction of larger files.

### Usage

To list files in a filesystem image:

```
./hw5 <image_file> ls
```

To copy a file from the image to your local disk:

```
./hw5 <image_file> cp <src_in_image> <dest_on_host>
```

For example: 
```
./hw5 fs.img cp readme.txt output.txt
```

## Build instructions

In order to compile the tool in Linux:

```bash
make
```

To run the automated tests (which takes in an already existing `fs.img`):

```
make test
```

## Notes
The root filesystem parsing supports file lists that are longer than one block *in theory*, but I did not test for that due to the lack of means and time... If it breaks I apologize.

This was a relatively fun one, I must admit. I love messing with the bytes in a file, and seeing cool results (See also: my [Brainfuck compiler](https://github.com/Starbuck7410/brainfuck-compiler))