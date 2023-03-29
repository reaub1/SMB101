#include<grp.h>
#include<pwd.h>
#include<sys/types.h>
#include<stdio.h>
#include<stdlib.h>
#include<dirent.h>

//main function
int main(int argc, char *argv[])
{

    //check the number of arguments
    if (argc != 2)
    {
        printf("Error on number of arguments\n");
        printf("Usage: %s <Path>\n",argv[1]);
        exit(1);
    }

    //show the file of the path
    struct dirent *dir;

    DIR *d = opendir(argv[1]);

    if(d)
    {
        while(dir=readdir(d))
        {
            printf("%s\n",dir->d_name);
        }
    }


    return 0;
}