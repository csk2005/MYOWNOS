Attempting to create my own OS and File System using C

## Features of OS:
- 16 bit OS
- 64 kb of ram

## Features of File System:
- Supports max disk size about 28 MB with upto 65535 files available having 64KB per file

- Superblocks - Metadata of File System as a whole (block count, inodes, magic numbers to indicate valididy)
- Inodes - Contains information about filename, size and what type of pointer (direct/indirect)
- Bitmap - To store what block is filled and what is not.
- Data Blocks - Actual data to be stored in the files.

### Make
```
make clean
make -C ../osapi clean
make
```

### Drive Commands (External)
```
#Create drive
dd if=/dev/zero of=disk.1 bs=512 count=65535 

#Check drive size
du -h

#View Binary drive data
hexdump ../drives/disk.1

#Format drives
../util/diskutil.exe format c
```