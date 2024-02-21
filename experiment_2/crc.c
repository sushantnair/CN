#include <stdio.h>
#include <string.h>
char dataword[100];         //This is the data that the user inputs, e.g., 10110
char generator[100];        //This is the generator that the user inputs, e.g., 1101
char crc_bits[100];         //This is the CRC bits generated at the sender side, e.g., 101
char Remainder[100];        //This is the remainder generated at the receiver side, e.g., 000
char codeword[100];         //This is the codeword that is sent by the sender, e.g., 10110101
int dataword_length, generator_length, i, j;
//function to perform XOR operation on two binary strings
void XOR()
{

    // 0 XOR 0 = 0; 0 XOR 1 = 1
    // 1 XOR 0 = 1; 1 XOR 1 = 0
    for(j = 1; j < generator_length; j++){
        crc_bits[j] = ((crc_bits[j] == generator[j]) ? '0' : '1');
    }
}
//function to implement CRC logic using XOR function
void CRC()
{
    for(i = 0; i < generator_length; i++)
        crc_bits[i] = dataword[i];
    do{
        //If the first bit of crc_bits is 1, then XOR with generator polynomial
        if(crc_bits[0] == '1')
            XOR();
        //Move the bits by 1 position for the next computation
        for(j = 0; j < generator_length - 1; j++)
            crc_bits[j] = crc_bits[j+1];
        //Appending a bit from initial codeword
        crc_bits[j] = dataword[i++];
    }while(i <= dataword_length + generator_length - 1);
}
void receiver()
{
    int flag;
    printf("\nEnter the received data (in binary form): ");
    scanf("%s", dataword);
    CRC();
    for(i = 0; i < strlen(crc_bits); i++){
        Remainder[i] = crc_bits[i];
    }
    printf("\nRemainder is: %s", Remainder);
    printf("\n----------------------------------------\n-----------------RESULT-----------------\n----------------------------------------\n");
    //Check if remainder is zero
    flag = 1;
    for(i = 0; i < strlen(crc_bits); i++){
        if(crc_bits[i] != '0')
            flag = 0;
    }
    if(flag == 1)
        printf("\nData integrity has been maintained.");
    else
        printf("\nData has been corrupted.");
}
int main()
{
    int rb;
    printf("\n----------------------------------\n--------------INPUTS--------------\n----------------------------------");
    printf("\nEnter a dataword (in binary form): ");
    scanf("%s", dataword);
    strcpy(codeword, dataword);
    printf("\nEnter a generator (in binary form): ");
    scanf("%s", generator);
    dataword_length = strlen(dataword);
    generator_length = strlen(generator);
    printf("\n--------------------------------------\n--------------PROCESSING--------------\n--------------------------------------\n");
    rb = generator_length - 1;
    printf("%d number of redundant bits shall be added to the codeword.\n", rb);
    while (rb > 0) {
        strcat(dataword, "0"); // Append '0' to init_codeword
        rb--;
    }
    printf("%s is the dataword padded with 0's.\n", dataword);
    //Sending initial codeword to CRC function 
    CRC();
    //Printing the crc bits
    printf("\n-------------------------------------\n--------------CRC  BITS--------------\n-------------------------------------\n");
    printf("\n\t%s", crc_bits);
    strcat(codeword, crc_bits);
    printf("\n------------------------------------\n--------------CODEWORD--------------\n------------------------------------\n");
    printf("\n\t%s", codeword);
    printf("\n-------------------------------------\n--------------SENT DATA--------------\n-------------------------------------\n");
    printf("\n\t%s", codeword);
    printf("\n-------------------------------------\n-------------------------------------\n-------------------------------------\n");
    receiver();

    return 0;
}