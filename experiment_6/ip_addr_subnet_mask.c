#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
int i;
char* dec_to_binary(int num)
{
    int rem, index = 0;     //remainder of division and index
    char temp_str[9] = {};  //8 binary digits + 1 null character
    while(num > 0){
        rem = num % 2;
        temp_str[index++] = rem + '0';
        //Convert the remainder to a character 0 or 1
        num = num / 2;
    }
    char* bin = (char*)malloc(index + 1);
    if(bin == NULL){
        perror("Memory Allocation Unsuccessful.");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < index; i++)
        bin[i] = temp_str[index - 1 - i];
    bin[index] = '\0';
    //Null-terminate the binary string
    return bin;
}
int bin_to_dec(char* bin)
{
    int bin_l = strlen(bin);
    int dec = 0, j;
    for(i = bin_l - 1, j = 0; i >= 0; i--, j++){
        if(bin[i] == '1'){
            dec = dec + (int)pow(2, j);
        }
    }
    return dec;
}
int func_call_cntr = 1;
int no_of_sub_bits(int nos, char nw_class)
{
    int prod = 1;
    int nosb;       //number of subnet bits
    for(i = 1; i <= nos; i++){
        prod = prod * 2;
        if(prod >= nos){
            nosb = i;
            break;
        }
    }
    if(func_call_cntr == 1){
        printf("\n\n--------------------------------------------------------------\n");
        printf("\nNumber of extra subnet bits is %d.", nosb);
        printf("\n\n--------------------------------------------------------------\n");
    }
    if(nw_class == 'A'){
        nosb += 8;
    } else if(nw_class == 'B'){
        nosb += 16;
    } else if(nw_class == 'C'){
        nosb += 24;
    } else {
        perror("Subnet Bits Overflow (too many number of subnet bits).");
        exit(EXIT_FAILURE);
    }
    if(func_call_cntr == 1)
        printf("\nTotal number of bits for network is %d.", nosb);
    func_call_cntr++;
    return nosb;
}
char* subnet_mask(int nos, char nw_class)
{
    int nosb;
    nosb = no_of_sub_bits(nos, nw_class);
    char* bin_subnet_mask = (char*)malloc(35);
    //32 bits + 3 periods
    if (bin_subnet_mask == NULL) {
        perror("Memory Allocation Unsuccessful.");
        exit(EXIT_FAILURE);
    }
    for(i = 0; i < nosb; i++){
        bin_subnet_mask[i] = '1';
    }
    for(i = nosb; i < 32; i++){
        bin_subnet_mask[i] = '0';
    }
    // Insert periods after every 8 bits (from left to right)
    for (i = 8; i < 32; i += 9) {
        for (int j = 32; j > i; j--) {
            bin_subnet_mask[j] = bin_subnet_mask[j - 1];
        }
        bin_subnet_mask[i] = '.';
    }
    
    // Ensure that the last byte has all zeros
    bin_subnet_mask[32] = '0';
    bin_subnet_mask[33] = '0';
    bin_subnet_mask[34] = '0';
    bin_subnet_mask[35] = '\0'; // Null-terminate the string properly
    return bin_subnet_mask;
}
char* bitwise_and(char* bin_ip_addr, char* bin_subnet_mask) {
    char* bin_result = (char*)malloc(38); // 32 bits + 3 periods + 1 null terminator
    if (bin_result == NULL) {
        perror("Memory Allocation Unsuccessful.");
        exit(EXIT_FAILURE);
    }

    int j = 0; // To keep track of the position in the result string
    for (i = 0; i < 32; i++) {
        if (bin_ip_addr[i] == '1' && bin_subnet_mask[i] == '1') {
            bin_result[j++] = '1';
        } else if (bin_ip_addr[i] == '.' || bin_subnet_mask[i] == '.') {
            bin_result[j++] = '.';
        } else {
            bin_result[j++] = '0';
        }
    }
    bin_result[j] = '\0'; // Null-terminate the string
    strcat(bin_result, "000");
    return bin_result;
}
char* broad_addr_fun(char* part, int noesb, int nosb)
{
    //printf("\nPart received %s", part);
    char* updated_part = (char*)malloc(9);
    for(i = 0; i < noesb; i++){
        updated_part[i] = part[i];
        //printf("updated part %c part %c", updated_part[i], part[i]);
    }
    for(i = noesb; i < nosb; i++){
        updated_part[i] = '1';
    }
    updated_part[8] = '\0';
    //printf("\nUpdated part %s", updated_part);
    return updated_part;
}
char* broadcast_addr(char* subnet_addr, int nos, char nw_class)
{
    int nosb, noesb;
    nosb = no_of_sub_bits(nos, nw_class);
    char broad_addr[36] = {};
    char part1[9], part2[9], part3[9], part4[9];
    strncpy(part1, subnet_addr, 8);
    part1[8] = '\0';
    strncpy(part2, subnet_addr + 9, 8);
    part2[8] = '\0';
    strncpy(part3, subnet_addr + 18, 8);
    part3[8] = '\0';
    strncpy(part4, subnet_addr + 27, 8);
    part4[8] = '\0';
    //printf("\nPart 1 is %s\nPart 2 is %s\nPart 3 is %s\nPart 4 is %s", part1, part2, part3, part4);
    if(nosb < 16){
        //Part 1 untouched, Part 2 functioned, Parts 3 and 4 made into all 1's
        noesb = nosb - 8;
        char* part2_new;
        char part3_new[9], part4_new[9];
        part2_new = broad_addr_fun(part2, noesb, nosb);
        memset(part3_new, '1', sizeof(part3_new) - 1);
        part3_new[8] = '\0';
        memset(part4_new, '1', sizeof(part4_new) - 1);
        part4_new[8] = '\0';
        strcat(broad_addr, part1);
        //printf("\nFirst step %.s", broad_addr);
        strcat(broad_addr, ".");
        //printf("\nSecond step %s", broad_addr);
        strcat(broad_addr, part2_new);
        //printf("\nThird step %s", broad_addr);
        strcat(broad_addr, ".");
        //printf("\nFourth step %s", broad_addr);
        strcat(broad_addr, part3_new);
        //printf("\nFifth step %s", broad_addr);
        strcat(broad_addr, ".");
        //printf("\nSixth step %s", broad_addr);
        strcat(broad_addr, part4_new);
        broad_addr[35] = '\0';
        //printf("\nSeventh step %s", broad_addr);

    } else if(nosb < 24){
        //Parts 1 and 2 untouched, Part 3 functioned, Part 4 made into all 1's
        noesb = nosb - 16;
        char* part3_new;
        char part4_new[9];
        part3_new = broad_addr_fun(part3, noesb, nosb);
        memset(part4_new, '1', sizeof(part4_new) - 1);
        part4_new[8] = '\0';
        strcat(broad_addr, part1);
        //printf("\nFirst step %.s", broad_addr);
        strcat(broad_addr, ".");
        //printf("\nSecond step %s", broad_addr);
        strcat(broad_addr, part2);
        //printf("\nThird step %s", broad_addr);
        strcat(broad_addr, ".");
        //printf("\nFourth step %s", broad_addr);
        strcat(broad_addr, part3_new);
        //printf("\nFifth step %s", broad_addr);
        strcat(broad_addr, ".");
        //printf("\nSixth step %s", broad_addr);
        strcat(broad_addr, part4_new);
        broad_addr[35] = '\0';
        //printf("\nSeventh step %s", broad_addr);
        //printf("\nNew part 3 %s", part3_new);
        //printf("\nNew part 4 %s", part4_new);
    } else if(nosb < 32){
        //Parts 1, 2 and 3 untouched, Part 4 functioned
        noesb = nosb - 24;
        char* part4_new;
        part4_new = broad_addr_fun(part4, noesb, nosb);
        strcat(broad_addr, part1);
        //printf("\nFirst step %.s", broad_addr);
        strcat(broad_addr, ".");
        //printf("\nSecond step %s", broad_addr);
        strcat(broad_addr, part2);
        //printf("\nThird step %s", broad_addr);
        strcat(broad_addr, ".");
        //printf("\nFourth step %s", broad_addr);
        strcat(broad_addr, part3);
        //printf("\nFifth step %s", broad_addr);
        strcat(broad_addr, ".");
        //printf("\nSixth step %s", broad_addr);
        strcat(broad_addr, part4_new);
        broad_addr[35] = '\0';
        //printf("\nSeventh step %s", broad_addr);
    }
    printf("\nFinal Broadcast address is %s", broad_addr);
    char bpart1[9], bpart2[9], bpart3[9], bpart4[9];    //bpart1 = broadcast part 1 = 11000000
    int b_part1, b_part2, b_part3, b_part4;             //b_part1 = broadcast part 1 = (int value)
    char str_part1[3], str_part2[3], str_part3[3], str_part4[3];
    char str_b_addr[16] = {};    //str_part1 = broadcast part 1 = (int to string value)
    printf("\n\n--------------------------------------------------------------\n");
    printf("\nBroadcast Address in decimal format is ");
    strncpy(bpart1, broad_addr, 8);
    bpart1[8] = '\0';
    b_part1 = bin_to_dec(bpart1);
    //printf("\n%d", b_part1);
    sprintf(str_part1, "%d", b_part1);
    printf("%s.", str_part1);
    strncpy(bpart2, broad_addr + 9, 8);
    bpart2[8] = '\0';
    b_part2 = bin_to_dec(bpart2);
    sprintf(str_part2, "%d", b_part2);
    printf("%s.", str_part2);
    strncpy(bpart3, broad_addr + 18, 8);
    bpart3[8] = '\0';
    b_part3 = bin_to_dec(bpart3);
    sprintf(str_part3, "%d", b_part3);
    printf("%s.", str_part3);
    strncpy(bpart4, broad_addr + 27, 8);
    bpart4[8] = '\0';
    b_part4 = bin_to_dec(bpart4);
    sprintf(str_part4, "%d", b_part4);
    printf("%s", str_part4);
    printf("\n\n--------------------------------------------------------------\n");
    printf("--------------------------------------------------------------\n");
    printf("--------------------------------------------------------------\n");
    //return broad_addr;
}
void main()
{
    char ip_addr[15], class[4], nw_class;
    char* ip_addr_parts;
    char* bin_ip_addr = NULL;
    char* bin_subnet_mask = NULL;
    char* bin_sub_msk = NULL;
    char* subnet_addr = NULL;
    int int_class;
    //Initialize to NULL
    printf("--------------------------------------------------------------\n--------------------IP ADDRESS & SUBNET MASK------------------\n--------------------------------------------------------------\n");
    printf("\nEnter the IP Address: ");
    scanf("%s", ip_addr);                                       // 140.179.220.200
    strncpy(class, ip_addr, 3);
    class[3] = '\0';
    int_class = atoi(class);
    printf("\n--------------------------------------------------------------\n");
    printf("\nIP Address Entered is %s", ip_addr);
    printf("\n\n--------------------------------------------------------------\n");
    if(int_class >= 1 && int_class <= 126){
        nw_class = 'A';
    } else if(int_class >= 128 && int_class <= 191){
        nw_class = 'B';
    } else if(int_class >= 192 && int_class <= 223){
        nw_class = 'C';
    } else if(int_class >= 224 && int_class <= 239){
        nw_class = 'D';
    } else {
        perror("Invalid IP Address. Terminating Program...");
        exit(EXIT_FAILURE);
    }
    printf("\nClass of IP Address for %d is %c", int_class, nw_class);
    printf("\n\n--------------------------------------------------------------");
    //Get the first part
    ip_addr_parts = strtok(ip_addr, ".");                       //140, 179, 220, 200
    //Keep getting parts until there are no more
    while(ip_addr_parts != NULL){
        //converting string to integer
        int int_ip_addr_parts;
        int_ip_addr_parts = atoi(ip_addr_parts);                // 140 (string) -> 140 (integer)
        //converting integer decimal to binary
        char* bin_ip_addr_parts;
        bin_ip_addr_parts = dec_to_binary(int_ip_addr_parts);   // 140 (decimal) -> 10001100
        //concatenating different parts into single binary IP Address
        if(bin_ip_addr == NULL){
            bin_ip_addr = strdup(bin_ip_addr_parts);
        } else {
            char* old_bin_ip_addr;
            old_bin_ip_addr = bin_ip_addr;
            bin_ip_addr = (char*)malloc(strlen(old_bin_ip_addr) + strlen(bin_ip_addr_parts) + 2);
            //2 for '.' and '\0'
            sprintf(bin_ip_addr, "%s.%s", old_bin_ip_addr, bin_ip_addr_parts);
            free(old_bin_ip_addr);
        }
        free(bin_ip_addr_parts);
        //Get the next part
        ip_addr_parts = strtok(NULL, ".");
    }
    printf("\n\nBinary form of IP Address is %.35s", bin_ip_addr);
    printf("\n\n--------------------------------------------------------------\n");
    int nos;
    printf("\nEnter the number of subnets to divide the network into: ");
    scanf("%d", &nos);
    bin_subnet_mask = subnet_mask(nos, nw_class);
    char* bin_subnet_mask_copy;
    bin_subnet_mask_copy = subnet_mask(nos, nw_class);
    printf("\n\n--------------------------------------------------------------\n");
    printf("\nSubnet Mask: %s", bin_subnet_mask);
    printf("\n\n--------------------------------------------------------------\n");
    // Print the subnet mask parts separately
    char* sub_msk_parts = strtok(bin_subnet_mask, ".");
    int part_count = 0;
    char str_sub_msk[16];
    strcpy(str_sub_msk, "");
    while(sub_msk_parts != NULL){
        //converting binary to decimal
        int int_sub_msk_parts;
        int_sub_msk_parts = bin_to_dec(sub_msk_parts);
        char str_sub_msk_parts[4];
        sprintf(str_sub_msk_parts, "%d", int_sub_msk_parts);
        strcat(str_sub_msk, str_sub_msk_parts);
        if(part_count < 3){
            strcat(str_sub_msk, ".");
        }
        part_count++;
        //Get the next part
        sub_msk_parts = strtok(NULL, ".");
    }
    printf("\nThe Subnet Mask in decimal notation is %s", str_sub_msk);
    printf("\n\n--------------------------------------------------------------\n");
    subnet_addr = bitwise_and(bin_ip_addr, bin_subnet_mask_copy);
    printf("\nThe Subnet Address is %s", subnet_addr);
    char spart1[9], spart2[9], spart3[9], spart4[9];    //spart1 = subnet part 1 = 11000000
    int s_part1, s_part2, s_part3, s_part4;             //s_part1 = subnet part 1 = (int value)
    char str_part1[3], str_part2[3], str_part3[3], str_part4[3];    //str_part1 = subnet part 1 = (int to string value)
    char str_s_addr[16] = {};    
    printf("\n\n--------------------------------------------------------------\n");
    printf("\nThe Subnet Address in decimal format is ");
    strncpy(spart1, subnet_addr, 8);
    spart1[8] = '\0';
    s_part1 = bin_to_dec(spart1);
    //printf("\n%d", b_part1);
    sprintf(str_part1, "%d", s_part1);
    printf("%s.", str_part1);
    strncpy(spart2, subnet_addr + 9, 8);
    spart2[8] = '\0';
    s_part2 = bin_to_dec(spart2);
    sprintf(str_part2, "%d", s_part2);
    printf("%s.", str_part2);
    strncpy(spart3, subnet_addr + 18, 8);
    spart3[8] = '\0';
    s_part3 = bin_to_dec(spart3);
    sprintf(str_part3, "%d", s_part3);
    printf("%s.", str_part3);
    strncpy(spart4, subnet_addr + 27, 8);
    spart4[8] = '\0';
    s_part4 = bin_to_dec(spart4);
    sprintf(str_part4, "%d", s_part4);
    printf("%s", str_part4);
    printf("\n\n--------------------------------------------------------------\n");
    broadcast_addr(subnet_addr, nos, nw_class);
}