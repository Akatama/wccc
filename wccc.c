#include <locale.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <wchar.h>
#include <wctype.h>

typedef struct {
    unsigned long byteCount;
    unsigned long charCount;
    unsigned long wordCount;
    unsigned long lineCount;
} File_Counter;

void print_help();

File_Counter wc(FILE * fptr);

int main(int argc, char **argv)
{
    FILE *fptr;

    bool byteflag = false;
    bool lineflag = false;
    bool wordflag = false;
    bool charflag = false;
    bool helpflag = false;

    char *fileName = NULL;
    int options;

    // Need to set locality for wide characters
    setlocale(LC_ALL, "");
    
    while (( options = getopt(argc, argv, "clwmh")) != -1)
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
            case 'h':
                helpflag = true;
                break;
            case '?':
                printf("Error Unknwon option %c\n", optopt);
                exit(EXIT_FAILURE);
        }

    if(helpflag)
    {
        print_help();
        exit(EXIT_SUCCESS);
    }

    if(!byteflag && !lineflag && !wordflag && !charflag)
    {
        byteflag = true;
        lineflag = true;
        wordflag = true;
    }

    if(optind < argc)
    {
        fileName = argv[optind];

        fptr = fopen(fileName, "r");
        if(fptr == NULL)
        {
            printf("Not able to open the file.\n");
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fptr = stdin;
    }
    

    File_Counter counter = wc(fptr);

    fclose(fptr);

    if(lineflag)
    {
        printf(" %ld", counter.lineCount);
    }

    if(wordflag)
    {
        printf(" %ld", counter.wordCount);
    }

    if(charflag)
    {
        printf(" %ld", counter.charCount);
    }

    if(byteflag)
    {
        printf(" %ld", counter.byteCount);
    }

    if(fileName)
    {
        printf(" %s", fileName);
    }

    printf("\n");

    return 0;
}


File_Counter wc(FILE *fptr)
{
    File_Counter counter = { .byteCount = 0, .lineCount = 0, .wordCount = 0, .charCount = 0 };
    bool isWord = false;
    wint_t ch;
    char buffer[8];
    while ((ch = fgetwc(fptr)) != WEOF)
    {
        counter.charCount++;

        //convert wide character to multibyte and add the length to the sum of bytes
        counter.byteCount += wctomb(buffer, ch);

        if (ch == L'\n')
        {
            counter.lineCount++;
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
                counter.wordCount++;
            }
        }


    }

    return counter;
}

/**
 * @brief Print help
 */
void print_help()
{
    printf("Usage: wccc [OPTION] ... [FILE]\n\n");
    printf("Print line, word, char and byte counts for FILE.\n\n");
    printf("-c\tprints the byte count\n");
    printf("-m\tprints the character count\n");
    printf("-l\tprints the word count\n");
    printf("-w\tprints the word count\n\n");
    printf("Without FILE, read standard intput\n");
}
