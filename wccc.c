#include <locale.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <wchar.h>
#include <wctype.h>

int wc(bool byteflag, bool lineflag, bool wordflag, bool charflag, char *fileName);

int main(int argc, char **argv)
{
    bool byteflag = false;
    bool lineflag = false;
    bool wordflag = false;
    bool charflag = false;

    char *fileName = NULL;
    int options;

    setlocale(LC_ALL, "");
    while (( options = getopt(argc, argv, "clwm")) != -1)
        switch(options)
        {
            case 'c':
                byteflag = true;
                break;
            case 'l':
                lineflag = true;
                break;
            case 'w':
                wordflag = true;
                break;
            case 'm':
                charflag = true;
                break;
            case '?':
                printf("Error Unknwon option %c\n", optopt);
                exit(EXIT_FAILURE);
        }
    if(optind < argc)
    {
        fileName = argv[optind];
    }
    else
    {
        printf("Must provide a file to use wccc\n");
        exit(1);
    }

    return wc(byteflag, lineflag, wordflag, charflag, fileName);

}


int wc(bool byteflag, bool lineflag, bool wordflag, bool charflag, char *fileName)
{
    if(wordflag || lineflag || charflag)
    {

        FILE *fptr = fopen(fileName, "r");
        if(fptr == NULL)
        {
            printf("Not able to open the file.\n");
            exit(1);
        }


        unsigned long wordCount = 0;
        unsigned long lineCount = 0;
        unsigned long charCount = 0;
        bool isWord = false;
        wint_t ch;
        while ((ch = fgetwc(fptr)) != WEOF)
        {
            charCount++;
            if (ch == L'\n')
            {
                lineCount++;
            }

            bool isSpace = iswspace(ch);

            if(isWord)
            {
                if(isSpace)
                {
                    //word has ended
                    isWord = false;
                }
            }
            else
            {
                if(!isSpace)
                {
                    // word has begun
                    isWord = true;
                    wordCount++;
                }
            }

            
        }
        
        if(lineflag)
        {
            printf(" %ld", lineCount);
        }

        if(wordflag)
        {
            printf(" %ld", wordCount);
        }

        if(charflag)
        {
            printf(" %ld", charCount);
        }

    }

    if(byteflag)
    {
        
        struct stat sb;
        if(stat(fileName, &sb) == -1)
        {
            printf("Error getting stats of file: %s\n", fileName);
            exit(1);
        }
        else
        {
            printf(" %lld", (long long) sb.st_size);
        }

    }
    printf(" %s\n", fileName);
    return 0;
}
