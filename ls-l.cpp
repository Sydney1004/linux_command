#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <string.h>

// simulate command ls -l
// -rwxr-xr-x   1 sydneyz  staff  139713 Apr 18 21:20 demo
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("%s filename\n", argv[0]);
        return -1;
    }

    struct stat st;
    int ret = stat(argv[1], &st);
    if (ret == -1) {
        perror("stat");
        return -1;
    }

    char rights[11] = {};

    switch(st.st_mode & S_IFMT) {
        case S_IFLNK:
            rights[0] = 'l';
            break;
        case S_IFDIR:
            rights[0] = 'd';
            break;
        case S_IFREG:
            rights[0] = '-';
            break;
        case S_IFBLK:
            rights[0] = 'b';
            break;
        case S_IFCHR:
            rights[0] = 'c';
            break;
        case S_IFSOCK:
            rights[0] = 's';
            break;
        case S_IFIFO:
            rights[0] = 'p';
            break;
        default:
            rights[0] = '?';
            break; 
    }

    rights[1] = st.st_mode & S_IRUSR ? 'r' : '-';
    rights[2] = st.st_mode & S_IWUSR ? 'w' : '-';
    rights[3] = st.st_mode & S_IXUSR ? 'x' : '-';

    rights[4] = st.st_mode & S_IRGRP ? 'r' : '-';
    rights[5] = st.st_mode & S_IWGRP ? 'w' : '-';
    rights[6] = st.st_mode & S_IXGRP ? 'x' : '-';

    rights[7] = st.st_mode & S_IROTH ? 'r' : '-';
    rights[8] = st.st_mode & S_IWOTH ? 'w' : '-';
    rights[9] = st.st_mode & S_IXOTH ? 'x' : '-';

    int link_num = st.st_nlink;

    char* owner = getpwuid(st.st_uid)->pw_name;

    char* group = getgrgid(st.st_gid)->gr_name;

    long int file_size = st.st_size;

    char* time = ctime(&st.st_mtime);
    
    char mtime[512] = {};
    strncpy(mtime, time, strlen(time) - 1);

    char buf[1024];

    sprintf(buf, "%s %d %s %s %ld %s %s", rights, link_num, owner, group, file_size, mtime, argv[1]);
    
    printf("%s\n", buf);

    return 0;
}