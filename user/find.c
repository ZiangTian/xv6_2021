#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

/*
#define T_DIR     1   // Directory
#define T_FILE    2   // File
#define T_DEVICE  3   // Device

struct stat {
  int dev;     // File system's disk device
  uint ino;    // Inode number
  short type;  // Type of file
  short nlink; // Number of links to file
  uint64 size; // Size of file in bytes
};

struct dirent {
  ushort inum; //  inode number, which is a unique identifier for the file or directory within the file system.
  char name[DIRSIZ]; // the name of the file/directoy
};


*/

static int has_found;

char* 
fmtname(char *path){
    static char buf[DIRSIZ+1];
    char *p;

    // Find first character after last slash.
    for(p=path+strlen(path); p >= path && *p != '/'; p--);
    p++;
    memmove(buf, p, strlen(p)+1); // strlen(p) returns the length of the file name
    return buf;
}

void 
find(char* path, char* fil){
    // check if the dir_path is a file or a a dir

    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if((fd = open(path, 0)) < 0){
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    if(fstat(fd, &st) < 0){
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return;
    } // return info of a file in the buffer pointed to by statbuf

    switch (st.type)
    {
    case T_FILE:
        if(strcmp(fmtname(path), fil) == 0){  // if using the original function, this would not be equal.
            printf("%s\n", path);
            has_found = 1;
        }
        // else, no action
        break;
    
    case T_DIR:

        if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
            printf("find: path too long\n");
            break;
        }
        // recursively look for target file

        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';

        while(read(fd, &de, sizeof(de)) == sizeof(de)){

            // note that de here is a dir entry contained in the given path
            // it is yet unclear whether de is a file or dir

            if((de.inum == 0) || (strcmp(de.name, ".") == 0) || (strcmp(de.name, "..") == 0 )) // likely unused or empty, skip
                continue;
            
            memmove(p, de.name, DIRSIZ); 
            
            p[DIRSIZ] = 0; // now buf contains the sub dir path

            find(buf, fil);
        }

        break;
    }
    close(fd);
}

int
main(int argc, char **argv)
{
    /* This is a simple one. Only two commandline args are allowed:
       a dir and a file name 
    */

    if(argc != 3){
        fprintf(2, "please put in a directory name and a file name.\n");
        exit(1);
    }

    char *dirp = argv[1];
    char *filen = argv[2];
    has_found = 0;

    find(dirp, filen);
    if(has_found == 0){
        printf("find: cannot find %s\n", filen);
    }
    
    exit(0);

}
