// gcc file_breakdown.c -lm

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define CHAR_LEN 8

char *char2bin_ch(char dst[8], unsigned char value)
{
    unsigned char bit;
    char *start = dst;
    
    /* Goes from most significant bit to least and gets 1 or 0 */
    for(bit = 1 << (CHAR_LEN - 1); bit; bit >>= 1)
        *dst++ = value & bit ? '1' : '0';
    *dst = '\0';
    return start;
}

/* Uses ints instead of chars */
unsigned short *char2bin_int(unsigned short dst[8], unsigned char value)
{
    unsigned char bit;
    unsigned short *start = dst;
    
    /* Goes from most significant bit to least and gets 1 or 0 */
    for(bit = 1 << (CHAR_LEN - 1); bit; bit >>= 1)
        *dst++ = value & bit ? 1 : 0;

    return start;
}

/* Uses ints instead of chars */
char bin2char(unsigned char bit[8])
{
    unsigned char int_char = 0;
    
    int bit_pos;
    for(bit_pos = 7; bit_pos >= 0; bit_pos--)
        int_char |= (*bit++ == '1' ? 1 << bit_pos : 0);

    return (char)int_char;
}


int main(int argc, char *argv[])
{
    if(argc == 3)
    {
        printf("\nOpening : %s\n",argv[1]);
        printf("Writing to : %s\n",argv[2]);

        FILE* input_file;
        FILE* output_file;
        input_file = fopen(argv[1],"r");
        output_file = fopen(argv[2],"wb");

        if(input_file != NULL)
        {
            char c;
            //while((c = fgetc(input_file)) != EOF) /* old method - didn't get EOF */
            while(fscanf(input_file,"%c",&c) == 1)
            {
                if(c != '\n')
                    printf("\n%c = ",c);
                else
                    printf("\n\'\\n\' = ");
                unsigned short bin[8];
                char2bin_int(bin,c);
                int i;
                for(i = 0; i < 8; i++)
                {
                    printf("%i",bin[i]);
                    fprintf(output_file,"%i",bin[i]);
                }
            } /* \n = 0A */
            printf("\n");

            close(input_file);
            close(output_file);

            /* Convert Back */
            //FILE* input_file;
            printf("\nOpening : %s\n",argv[2]);
            //            ReadFile(argv[2]);
            //input_file(fopen(argv[1],"rb", output_file);
            freopen(argv[2],"r", output_file);
            if(output_file != NULL)
            {
                char test = 'a';
                unsigned char bin_char[8];
                unsigned char* ptr = bin_char;

                while(fscanf(output_file,"%c%c%c%c%c%c%c%c",ptr++,ptr++,ptr++,ptr++,ptr++,ptr++,ptr++,ptr++) == 8)
                {
                    char c = bin2char(bin_char);
                    printf("\n%s : ",bin_char);
                    if(c != '\n')
                        printf("%c",c);
                    else
                        printf("\'\\n\'");
                    //ptr = bin_char+7;
                    ptr = bin_char;
                }
                printf("\n");
                close(output_file);
            }
        }
        else
            return -1; // When in code call error here so it exits like a throw
    }
    else
    printf("Invalid number of arguements\n");

    return 0;
}
