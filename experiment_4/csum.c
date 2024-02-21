#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//Function to split the input string into parts of four characters each
char** string_splitter(char* input_string)
{
    int len, nop;
    len = strlen(input_string);
    nop = len / 4;  //Calculate the number of parts needed
                    //For a string of 16 characters, the number 
                    //of parts needed is 4.
    char** parts = (char**)malloc(nop * sizeof(char*));
    for(int i = 0; i < nop; i++){
        int part_length = 4;
        if(i == nop - 1){
            //Adjust the length for the last part
            part_length = len % 4;
            if(part_length == 0)
                part_length = 4;
        }
        parts[i] = (char*)malloc((part_length + 1) * sizeof(char));
        strncpy(parts[i], input_string + i * 4, part_length);
        parts[i][part_length] = '\0'; //Terminating character of each part
    }
    return parts;
}
//Function to convert a string to hexadecimal and add the parts
char* hex_addition(char* input_string)
{
    char** parts = string_splitter(input_string);
    int nop;
    nop = strlen(input_string) / 4 + (strlen(input_string) % 4 == 0 ? 0 : 1);
    unsigned int sum = 0;
    for(int i = 0; i < nop; i++){
        unsigned int part_value;
        sscanf(parts[i], "%X", &part_value);
        sum += part_value;
    }
    char* result = (char*)malloc(9 * sizeof(char));
    //Maximum 8 characters for a 32-bit hexadecimal number
    sprintf(result, "%X", sum);
    return result;
}
//Function to add carry to the intermediate sum and truncate to 4 bits
char* final_sum(char* intermediate_sum)
{
    int carry = 0;
    int sum;
    sum = strtol(intermediate_sum, NULL, 16);
    //Convert the sum part to an integer
    //Extract the carry if it is present
    if(strlen(intermediate_sum) > 4)
        carry = strtol(intermediate_sum, NULL, 16) >> 16;
    //Add the carry to the sum
    sum += carry;
    //Truncate to 4 bits
    sum &= 0xFFFF;
    char* result = (char*)malloc(5 * sizeof(char));
    //4 bits for the sum
    sprintf(result, "%X", sum);
    return result;
}
//Function to complement the final result to give the checksum
char* complementer(char* final_result)
{
    int check_sum;
    check_sum = strtol(final_result, NULL, 16);
    //Perform bitwise NOT on each nibble (4 bits)
    check_sum ^= 0xFFFF;
    char* result = (char*)malloc(5 * sizeof(char));
    //4 bits for the checksum
    sprintf(result, "%X", check_sum);
    return result;
}
void receiver()
{
    char str_received_data[100];
    printf("\n\n------------------------------------------------\n--------------------RECEIVER--------------------\n------------------------------------------------\n");
    printf("\nEnter Received Data: ");
    scanf("%s", str_received_data);
    char* received_data = str_received_data;
    char* recv_int_hex_sum = hex_addition(received_data);
    printf("\nIntermediate Hexadecimal Sum of the parts: %s", recv_int_hex_sum);
    char* recv_sum_with_carry = final_sum(recv_int_hex_sum);
    printf("\nFinal Hexadecimal Sum with Carry Added: %s", recv_sum_with_carry);
    char* recv_complement = complementer(recv_sum_with_carry);
    printf("\nComplement of Final Hexadecimal Sum: %s", recv_complement);
    int flag = 1;
    for(int i = 0; i < strlen(recv_complement); i++){
        if(recv_complement[i] != '0')
            flag = 0;
            break;
    }
    if(flag == 1)
        printf("\nData integrity check is successful.");
    else
        printf("\nData has been corrupted.");
}
int main()
{
    char str_input_data[100];
    char send_data[100];
    printf("--------------------------------------------------------\n--------------------CHECKSUM PROGRAM--------------------\n--------------------------------------------------------\n");
    printf("\n\n\n---------------------------------------------\n--------------------INPUT--------------------\n---------------------------------------------\n");
    printf("\nEnter packet header as a single hexadecimal string without spaces in between: ");
    scanf("%s", str_input_data);
    printf("\n----------------------------------------------\n--------------------SENDER--------------------\n----------------------------------------------\n");
    char* input_data = str_input_data;
    char* int_hex_sum = hex_addition(input_data);
    printf("\nIntermediate Hexadecimal Sum of the parts: %s", int_hex_sum);
    char* sum_with_carry = final_sum(int_hex_sum);
    printf("\nFinal Hexadecimal Sum with Carry Added: %s", sum_with_carry);
    char* check_sum = complementer(sum_with_carry);
    printf("\nChecksum: %s", check_sum);
    strcat(send_data, input_data);
    strcat(send_data, check_sum);
    printf("\nData Sent: ");
    for(int i = 0; i < strlen(send_data); i++){
        printf("%c", send_data[i]);
        if(i % 4 == 3){
            printf("    ");
        }
    }
    receiver();
    printf("\n\n----------------------------------------------------\n--------------------PROGRAM ENDS--------------------\n----------------------------------------------------");
    return 0;
}