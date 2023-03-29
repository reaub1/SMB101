#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

#define STDOUT 1
#define MAX_PATH_LENGTH 4096
 

void display(const char* name)
{
    struct stat st;
    if(stat(name, &st)== -1)
    {
        perror("stat");
        exit(1);
    }


    struct passwd *pw = getpwuid(st.st_uid);

    struct group *gr = getgrgid(st.st_gid);

    char mode[9]="---------";

    if( st.st_mode & S_IRUSR )
        //printf("r");
        mode[0]='r';
    if( st.st_mode & S_IWUSR )
        //printf("w");
        mode[1]='w';
    if( st.st_mode & S_IXUSR )
        //printf("x");
        mode[2]='x';
    if( st.st_mode & S_IRGRP )
        //printf("r");
        mode[3]='r';
    if( st.st_mode & S_IWGRP )
        //printf("w");
        mode[4]='w';
    if( st.st_mode & S_IXGRP )
        //printf("x");
        mode[5]='x';
    if( st.st_mode & S_IROTH )
        //printf("r");
        mode[6]='r';
    if( st.st_mode & S_IWOTH )
        //printf("w");
        mode[7]='w';
    if( st.st_mode & S_IXOTH )
        //printf("x");
        mode[8]='x';

    printf("%s -  %s - %s:%s\n", name,mode, pw->pw_name, gr->gr_name);
} 
int main(int argc, char** argv)
{
    DIR *d;
    struct dirent *dir;
    d = opendir(getenv("PWD"));
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            display(dir->d_name);
        }
        closedir(d);
    }
    return(0);
}