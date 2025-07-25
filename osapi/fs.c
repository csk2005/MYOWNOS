/* fs.c */
#include "fs.h"

internal bitmap *mkbitmap(filesystem *fs, bool scan) {
    bitmap *bm;
    int16 size;
    int16 n, x;
    fsblock block;
    bool ret;
    int16 index;
    bool valid;

    if(!fs||!fs->dd) return (bitmap *)false;

    size = (fs->dd->blocks + 7) / 8;
    bm = (bitmap *)alloc($i size);
    if(!bm) return (bitmap *)false;

    zero($1 bm, size);
    if(!scan)return bm;

    index = 0;
    for (n=2; n<= (fs->metadata.inodeblocks+1); n++) {
        zero($1 &block, Blocksize);
        ret = dread(fs->dd, &block, n);
        if(!ret) {
            destroy(bm);
            return (bitmap *)false;
        }

        for (x=0; x<InodesPerBlock; x++) {
            valid = (block.inodes[x].validtype & 0x01);
            if(valid)setbit($1 bm, index++, true);
            else setbit($1 bm, index++, false);
        }
    }

    return bm;

}

internal int16 blockbitmapalloc(filesystem* fs, bitmap *bm){
    int16 n;
    int16 bl;

    if(!fs || !fs->dd || !bm) return 0;

    for(n=0;n<fs->dd->blocks;n++){
        if(!getbit($1 bm, n)){
            setbit($1 bm, n, true);
            bl = (n+1);

            return bl;
        }
    }
    return 0;
}

internal void bitmapfree(filesystem* fs, bitmap *bm, int16 bl){
    int16 n;

    if(!fs || !bm) return;

    n= (bl-1);

    setbit($1 bm, n, false);
    return;
}

filesystem *fsformat(disk *dd, bootsector *mbr, bool force){
    filesystem *fs;
    int16 size;
    int16 inodeblocks;
    int16 blocks;
    superblock super;
    inode idx;
    block idxblock;
    bool ok;
    int16 n;
    fsblock fsb;
    bitmap *bm;

    if(!dd) reterr(ErrNotAttached);
    
    if(openfiles(dd)){
        if(!force)reterr(ErrBusy);
        else closeallfiles(dd);
    }  
    
    blocks = dd->blocks;
    inodeblocks = (blocks+9) / 10;
    idx.validtype = TypeDir;
    idx.size = 0;
    zero($1 &idx.name, 11);
    idx.indirect = 0;
    size = (sizeof(ptr)*PtrPerInode);
    zero($1 &idx.direct, size);

    super.magic1 = Magic1;
    super.magic2 = Magic2;
    super.inodes = 1;
    super.inodeblocks = inodeblocks;
    super.blocks = blocks;
    super._ = 0;

    if(mbr)copy($1 &super.boot,$1 mbr, 500);
    else zero($1 &super.boot, 500); 
    
    ok = dwrite(dd,&super, 1);
    if(!ok) reterr(ErrIO);

    zero($1 &idxblock, Blocksize);
    copy($1 &idxblock, $1 &idx, sizeof(idx));

    ok = dwrite(dd, &idxblock, 2);
    if(!ok) reterr(ErrIO);

    zero($1 &fsb, Blocksize);
    for(n=0; n<inodeblocks; n++){
        ok = dwrite(dd, &fsb, n+3);
        if(!ok) reterr(ErrIO);
    }

    size = sizeof(struct s_filesystem);
    fs = (filesystem *)alloc(size);
    if(!fs) reterr(ErrNoMem);
    zero($1 fs, size);

    fs->drive = dd->drive;
    fs->dd = dd;
    copy($1 &fs->metadata, $1 &super, Blocksize);

    bm = mkbitmap(fs, false);
    size = 1+1+fs->metadata.inodeblocks;
    for(n=0; n<size; n++){
        setbit($1 bm, n, true);
    }
    fs->bitmap = bm;

    fsshow(fs, true);
    
    return fs;
}

internal void fsshow(filesystem *fs, bool showbm){
    int8 drivechar;
    ptr inodeno, n;
    inode *ino;
    
    if(!fs)return;

    if(fs->drive == 1||fs->drive == 2){
        drivechar = (int8)('c' + fs->drive - 1);
    }
    else drivechar = '?';

    printf("Disk 0x%.02hhx, mounted on %c:\n",(char)fs->drive, (char)drivechar);

    printf("  %d total blocks, 1 superblock and %d inode blocks\n    containing %d inodes\n\n",
         $i fs->dd->blocks, $i fs->metadata.inodeblocks, $i fs->metadata.inodes);
    
    for(inodeno=0; inodeno<fs->metadata.inodes; inodeno++){
        ino = findinode(fs, inodeno);
        if(!ino) break;

        if((ino->validtype & 0x01)){
            printf("Inode %d is valid\n  type: %s\n  name: %s\n  size: %d bytes\n",
                $i inodeno, (ino->validtype == TypeFile)? "File" : (ino->validtype == TypeDir)? "Directory" : "Unknown",
                (!inodeno)? "/" : $c file2str(&ino->name), $i ino->size);
        }
    }
    printf("\n");

    if(showbm && fs->bitmap){
        printf("Bitmap: \n");
        fflush(stdout);
        for(n = 0; n < fs->metadata.blocks; n++){
            printf("%d", getbit($1 fs->bitmap, n));
            if((n+1) % 8 == 0) printf(" ");
            if((n+1) % 64 == 0) printf("\n");
        } 
    }
    printf("\n\n");

}

internal inode *findinode(filesystem* fs, ptr idx){
    fsblock bl;
    int16 n, size;
    bool res;
    inode* ret;
    ptr x, y;

    if(!fs) return (inode *)0;

    ret = (inode *)0;
    for(n=0,x=2; x<(fs->metadata.inodeblocks); x++){
        zero($1 &bl, Blocksize);
        res = dread(fs->dd, $1 &bl.data, x);

        if(!res) return ret;

        for(y=0;y<InodesPerBlock; y++){
            if(n==idx){
                size = sizeof(inode);
                ret = (inode *)alloc(size);
                if(!ret) return ret;
                zero($1 ret, size);
                copy($1 ret, $1 &bl.inodes[y], size);
                return ret;
            }
            n++;
        }
    }
    return ret;
}

internal int8 *file2str(filename *fname){
    static int8 buf[16];
    int8 *p;
    int16 n;
    if(!(*fname->ext)) return fname->name;
    zero($1 buf, 16);

    strcopy($1 &buf,$1 fname->name, 8);
    n = stringlen(buf);
    n++;
    buf[n++] = '.';
    p = buf+n;

    strcopy(p, fname->ext, 3);
    p=buf+n;

    return p;
} 

extern public disk *DiskDescriptor[Maxdrive];
public filesystem *FSdescriptor[Maxdrive];
internal filesystem *fsmount(int8 drive){
    ptr idx;
    disk *dd;
    filesystem *fs;
    int16 size;

    if((drive < 1) || (drive > Maxdrive)) return (filesystem *)0;
    idx = drive - 1;

    dd = DiskDescriptor[idx];
    if(!dd) return (filesystem *)0;

    size = sizeof(filesystem);
    fs = (filesystem *)alloc(size);

    fs = (filesystem *)alloc(size);
    if(!fs) return (filesystem *)0;
    zero($1 fs, size);

    fs->drive = drive;
    fs->dd = dd;
    fs->bitmap = mkbitmap(fs, true);
    if(!fs->bitmap) {
        destroy(fs);
        return (filesystem *)0;
    }

    if(!dread(fs->dd, &fs->metadata, 1)) {
        destroy(fs);
        return (filesystem *)0;
    }
    osprintf("Mounted disk 0x%x on drive %c:\n", drive, (drive==1)? 'c' : ((drive==2)? 'd' : '?'));

    FSdescriptor[idx] = fs;

    return fs;

}

internal void fsunmount(filesystem *fs){
    ptr idx;
    int16 drive;

    if(!fs) return;

    drive = fs->dd->drive;
    idx  = (drive-1);
    FSdescriptor[idx] = (filesystem *)0;
    destroy(fs);

    osprintf("Unmounted drive %c:", (drive==1)?  'c' : ((drive==2)? 'd' : '?'));

    return;

}