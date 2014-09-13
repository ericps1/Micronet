// gcc file_breakdown.c -lm

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <math.h>

#define PORT 3490
#define MAXSIZE 1024

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
    struct sockaddr_in server_info;
    struct hostent *he;
    int socket_fd,num;
    char buffer[1024];

    if (argc != 3) {
        fprintf(stderr, "Usage: client hostname\n");
        exit(1);
    }

    if ((he = gethostbyname(argv[2]))==NULL) {
        fprintf(stderr, "Cannot get host name\n");
        exit(1);
    }

    if ((socket_fd = socket(AF_INET, SOCK_STREAM, 0))== -1) {
        fprintf(stderr, "Socket Failure!!\n");
        exit(1);
    }

    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    server_info.sin_port = htons(PORT);
    server_info.sin_addr = *((struct in_addr *)he->h_addr);
    if (connect(socket_fd, (struct sockaddr *)&server_info, sizeof(struct sockaddr))<0) {
        //fprintf(stderr, "Connection Failure\n");
        perror("connect");
        exit(1);
    }

    if(argc == 3)
    {
        printf("\nOpening : %s\n",argv[1]);

        FILE* input_file;
        input_file = fopen(argv[1],"r");

        if(input_file != NULL)
        {
            int c;
            while(fread(&c,1,1,input_file))
            {
                if ((send(socket_fd,&c, 1,0))== -1) 
                {
                    fprintf(stderr, "Failure Sending Message\n");
                    close(socket_fd);
                    exit(1);
                }
                else 
                {
                    printf("Char to send %c\n",c);
                    num = recv(socket_fd, buffer, sizeof(buffer),0);
                    if ( num <= 0 )
                    {
                        printf("Either Connection Closed or Error\n");
                        //Break from the While
                        break;
                    }

                    /* Do checks on what the received data was to see 
                     * if changes need to be made to the program running */
                    /* could have set up config flies transmitted to the 
                     * RPI to get the frequency and everything set */

                    buffer[num] = '\0';
                    printf("Client:Message Received From Server -  %s\n",buffer);
                }
            } /* \n = 0A */
            printf("\n");

            close(socket_fd);
            fclose(input_file);
        }
        else
            return -1; // When in code call error here so it exits like a throw
    }
    else
    printf("Invalid number of arguements\n");

    return 0;
}
