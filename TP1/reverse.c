#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>

const int MAX_LINE_LENGTH = 1024;
const int STDERR = 1;


int main(int argc, char *argv[])
{
    // check if the number of arguments is correct
    if (argc != 2)
    {
        printf("Error on number of arguments\n");
        printf("Usage: %s <input file txt>\n",argv[1]);
        exit(STDERR);
    }

    // open the input file
    regex_t reegex;

    int value = regcomp(&reegex, "[:.txt:]",0); 
    value = regexec( &reegex, argv[1],  0, NULL, 0);

    if (value != 0)
    {
        printf("Error on the regex matching\n");
        printf("Usage: %s <input file txt>\n",argv[1]);
        exit(STDERR);
    }

    FILE *input_file = fopen(argv[1], "r");
    if (input_file == NULL)
    {
        printf("Error: cannot open file %s", argv[1]);
        exit(STDERR);
    }

    //write the reversed lines on standard output
    char line[MAX_LINE_LENGTH];
    while (fgets(line, MAX_LINE_LENGTH, input_file) != NULL)
    {
        int len = strlen(line);
        for (int i = len - 1; i >= 0; i--)
        {
            printf("%c", line[i]);
        }
    }

    printf("\n");
}
