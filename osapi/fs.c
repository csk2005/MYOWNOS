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
            if(valid)bm[index++] = true;
            else bm[index++] = false;
        }
    }

    return bm;

}

internal int16 blockbitmapalloc(filesystem* fs, bitmap *bm){
    int16 n;
    int16 bl;

    if(!fs || !fs->dd || !bm) return 0;

    for(n=0;n<fs->dd->blocks;n++){
        if(!bm[n]){
            bm[n] = true;
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

    bm[n] = false;
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
        bm[n]=true;
    }
    fs->bitmap = bm;

    return fs;
}