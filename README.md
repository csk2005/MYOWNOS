Attempting to create my own OS and File System using C

```
make clean
make -C ../osapi clean
make

dd if=/dev/zero of=disk.1 bs=512 count=65535 
//to create drive

du -h
//to check drive size

hexdump ../drives/disk.1
//to view drive data

```