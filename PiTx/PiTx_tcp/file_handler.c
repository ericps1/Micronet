#include "file_handler.h"

/********************************************************************/
/*																	*/
/*  Function :       		                            */
/*																	*/
/*	Arguements :    					    					*/
/*																	*/
/*	Purpose :                 */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
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

/********************************************************************/
/*																	*/
/*  Function :       		                            */
/*																	*/
/*	Arguements :    					    					*/
/*																	*/
/*	Purpose :                 */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
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

/********************************************************************/
/*																	*/
/*  Function :       		                            */
/*																	*/
/*	Arguements :    					    					*/
/*																	*/
/*	Purpose :                 */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
/* Uses ints instead of chars */
char bin2char(unsigned char bit[8])
{
    unsigned char int_char = 0;
    
    int bit_pos;
    for(bit_pos = 7; bit_pos >= 0; bit_pos--)
        int_char |= (*bit++ == '1' ? 1 << bit_pos : 0);

    return (char)int_char;
}

/********************************************************************/
/*																	*/
/*  Function : file_breakdown      		                            */
/*																	*/
/*	Arguements :    					    					*/
/*																	*/
/*	Purpose : Read in a text file and                */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
int file_breakdown(char* file_name, int** file_data)
{
    int len = 0;

    if(strstr(file_name,".txt")!=NULL)
    {
        FILE* input_file;
        input_file = fopen(file_name,"r");

        if(verbose)
            printf("File Breakdown\n");

        free(*file_data);
        *file_data = NULL;
        *file_data = malloc(FILE_DATA_BLOCK*sizeof(int));
        if(*file_data == NULL)
            error("File Data Len memory could not be allocated");
        //int file_data[FILE_DATA_BLOCK];
        char c[FILE_DATA_BLOCK];

        if(input_file != NULL)
        {
            if(verbose)
                printf("File opened\n");

            while(fscanf(input_file,"%c",&c[len/8]) == 1 && len+8 < FILE_DATA_BLOCK)
            {
                unsigned short bin[8];
                char2bin_int(bin,c[len/8]);

                int i;
                for(i = 0; i < 8; i++)
                    (*file_data)[len+i] = bin[i];

                len += 8;
            } /* \n = 0A */

            if(verbose)
            {
                printf("\n");

                printf("Data from file : ");
                int i;
                for(i = 0; i < len; i++)
                {
                    if(!(i%8))
                    {
                        if(c[i/8] != '\n')
                            printf("\n%c = ",c[i/8]);
                        else
                            printf("\n\\n = ");
                    }
                    printf("%i",(*file_data)[i]);
                }
                printf("\n");
            }

            fclose(input_file);
        }
    }
    return len;
}

/********************************************************************/
/*																	*/
/*  Function : string_breakdown      		                            */
/*																	*/
/*	Arguements :    					    					*/
/*																	*/
/*	Purpose : Read in a string and convert it to binary ints                */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
int* string_breakdown(char* in)
{
    int len = strlen(in);
    int* data = malloc(len*8*sizeof(int));

    if(verbose)
        printf("File opened\n");

    int curr_char;
    for(curr_char = 0; curr_char < len; curr_char++)
    {
        unsigned short bin[8];
        char2bin_int(bin,in[curr_char]);

        int i;
        for(i = 0; i < 8; i++)
            data[curr_char*8+i] = bin[i];
    } /* \n = 0A */

    if(verbose)
    {
        printf("\n");

        printf("Data from string : ");
        int i;
        for(i = 0; i < len*8; i++)
        {
            if(!(i%8))
            {
                if(in[i/8] != '\n')
                    printf("\n%c = ",in[i/8]);
                else
                    printf("\n\\n = ");
            }
            printf("%i",data[i]);
        }
        printf("\n");
    }

    return data;
}
