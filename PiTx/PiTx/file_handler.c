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
int file_breakdown(char* file_name, int** file_data, int curr_block)
{
    int len = 0;
    static int pos;

    if(strstr(file_name,".txt")!=NULL)
    {
        FILE* input_file;
        input_file = fopen(file_name,"r");

        /* Make sure the file opened successfully */
        if(input_file != NULL)
        {
            if(verbose)
                printf("File Breakdown\n");

            if(curr_block)
                fseek(input_file, 0, SEEK_END);

            if(pos != ftell(input_file) && curr_block)
                fseek(input_file, --pos, SEEK_SET);
            else if(curr_block)
                return 0;

            //if(curr_block)
            //    free((*file_data));
            (*file_data) = NULL;
            (*file_data) = (int*)malloc(FILE_DATA_BLOCK*sizeof(int));
            if(*file_data == NULL)
                error("File Data Len memory could not be allocated");
            //int file_data[FILE_DATA_BLOCK];
            char c[FILE_DATA_BLOCK];

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
                //printf("\n");

                //printf("Data from file : ");
                //int i;
                //for(i = 0; i < len; i++)
                //{
                //    if(!(i%8))
                //    {
                //        if(c[i/8] != '\n')
                //            printf("\n%c = ",c[i/8]);
                //        else
                //            printf("\n\\n = ");
                //    }
                //    printf("%i",(*file_data)[i]);
                //}
                //printf("\n");
            }

            pos = ftell(input_file);

            fclose(input_file);
        }
    }

    return len;
}

/********************************************************************/
/*																	*/
/*  Function : binary_file_breakdown      		                            */
/*																	*/
/*	Arguements :    					    					*/
/*																	*/
/*	Purpose : Read in a text file and                */
/*                                                                  */ 
/*  Example :                                       */
/*																	*/
/********************************************************************/
int binary_file_breakdown(char* file_name, int** file_data, int curr_block)
{
    int len = 0;
    static int bin_pos;

    if(strstr(file_name,".txt")!=NULL)
    {
        FILE* input_file;
        input_file = fopen(file_name,"r");

        /* Make sure the file opened successfully */
        if(input_file != NULL)
        {
            if(verbose)
                printf("Binary File Breakdown\n");

            if(curr_block)
                fseek(input_file, 0, SEEK_END);

            if(bin_pos != ftell(input_file) && curr_block)
                fseek(input_file, --bin_pos, SEEK_SET);
            else if(curr_block)
                return 0;

            //if(curr_block)
            //    free((*file_data));
            (*file_data) = NULL;
            (*file_data) = (int*)malloc(FILE_DATA_BLOCK*sizeof(int));
            if(*file_data == NULL)
                error("File Data Len memory could not be allocated");
            //int file_data[FILE_DATA_BLOCK];
            char c[FILE_DATA_BLOCK];

            if(verbose)
                printf("File opened\n");

            while(fscanf(input_file,"%c",&c[len]) == 1 && len+1 < FILE_DATA_BLOCK)
            {
                if(c[len] != '0')
                    (*file_data)[len] = 1;
                else
                    (*file_data)[len] = 0;

                len++;
            } /* \n = 0A */

            if(verbose)
            {
                //printf("\n");

                printf("Data from file : ");
                int i;
                for(i = 0; i < len; i++)
                {
                    printf("%i",(*file_data)[i]);
                }
                printf("\n");
            }

            bin_pos = ftell(input_file);

            fclose(input_file);
        }
    }

    return len;
}




