#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <ctype.h>
#include <Windows.h>

bool check_prime(long long int num)
{
    if (num <= 1)
        return false;
    for (long long int i = 2; i * i <= num; i++)
    {
        if (num % i == 0)
            return false;
    }
    return true;
}

long long int generate_random_prime_number(long long int lower_limit, long long int upper_limit)
{
    long long int range = upper_limit - lower_limit + 1;
    long long int random_num = rand() % range + lower_limit;
    while (!check_prime(random_num))
        random_num = rand() % range + lower_limit;
    return random_num;
}

long long int compute_secure_key(long long int base, int exponent, long long int modulus)
{
    long long int result = 1;
    for (int i = 0; i < exponent; i++)
        result = (result % modulus) * base;
    return result % modulus;
}

void encrypt_file(const char *filename, long long symmetric_key) {
    int division_count = 0;
    FILE *input_file, *output_file;
    char ch;
    long long key_copy = symmetric_key;
    input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    output_file = fopen("encrypted.txt", "w");
    if (output_file == NULL) {
        printf("Error creating the encrypted file.\n");
        fclose(input_file);
        return;
    }
    while ((ch = fgetc(input_file)) != EOF) {
        if (!isspace(ch)) {
            ch += (key_copy % 10); // Encrypting the character

            // Wrap around if necessary
            if (ch > 127)
                ch = 32 + (ch - 127);
            else if (ch < 32)
                ch = 127 - (32 - ch);

            key_copy /= 10; // Move to the next digit in the key
            division_count++;
            if (division_count > 7)
            {
                key_copy = symmetric_key;
                division_count = 0;
            }
        }
        fputc(ch, output_file);
        if (isspace(ch)) {
            // Restore the key
            key_copy = symmetric_key;
        }
    }
    fclose(input_file);
    fclose(output_file);
    printf("File encrypted successfully!\n");
}

void decrypt_file(const char *filename, long long symmetric_key) {
    FILE *input_file, *output_file;
    char ch;
    int division_count = 0;
    long long key_copy = symmetric_key;

    input_file = fopen(filename, "r");
    if (input_file == NULL) {
        printf("Error opening the file.\n");
        return;
    }
    output_file = fopen("decrypted.txt", "w");
    if (output_file == NULL) {
        printf("Error creating the decrypted file.\n");
        fclose(input_file);
        return;
    }
    while ((ch = fgetc(input_file)) != EOF) {
        if (!isspace(ch)) {
            ch -= (key_copy % 10); // Decrypting the character
            // Wrap around if necessary
            if (ch < 32)
                ch = 127 - (32 - ch);
            else if (ch > 127)
                ch = 32 + (ch - 127);

            key_copy /= 10; // Move to the next digit in the key
            division_count++;
            if (division_count > 7)
            {
                key_copy = symmetric_key;
                division_count = 0;
            }
        }
        fputc(ch, output_file);
        if (isspace(ch)) {
            // Restore the key
            key_copy = symmetric_key;
        }
    }

    fclose(input_file);
    fclose(output_file);
    printf("File decrypted successfully!\n");
}

void clear_console()
{
    system("cls");
}

void change_text_color(WORD color)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

void print_header(const char *text)
{
    change_text_color(FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    printf("\n%s\n", text);
    change_text_color(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

void print_info(const char *text)
{
    change_text_color(FOREGROUND_GREEN);
    printf("%s", text);
    change_text_color(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}

int main()

{
    int alice_private_key, bob_private_key;
    long long int prime_g, prime_n, lower_limit = 1000000000, upper_limit = 9999999999; // Lower and upper limits for prime number generation
    srand(time(NULL));
    prime_g = generate_random_prime_number(lower_limit, upper_limit);
    prime_n = generate_random_prime_number(lower_limit, upper_limit);
    print_header("Secure Communication Program");
    printf("\n");
    printf("Randomly generated big prime numbers are %lld and %lld\n", prime_g, prime_n);
    // Sleep(1000);
    printf("Enter the private key for Alice: ");
    scanf("%d", &alice_private_key);
    printf("Enter the private key for Bob: "); // Only Alice knows x and only Bob knows y
    scanf("%d", &bob_private_key);
    long long int alice_public_key = compute_secure_key(prime_g, alice_private_key, prime_n);
    long long int bob_public_key = compute_secure_key(prime_g, bob_private_key, prime_n); // Both Alice and Bob exchange A and B via a public domain.
    Sleep(500);
    printf("\nPublic key generated by Alice: %lld\n", alice_public_key);
    printf("Public key generated by Bob: %lld\n", bob_public_key);
    Sleep(500);
    printf("\nSecure keys generated by Alice and Bob are: ");
    long long int alice_symmetric_key = compute_secure_key(bob_public_key, alice_private_key, prime_n);
    long long int bob_symmetric_key = compute_secure_key(alice_public_key, bob_private_key, prime_n); // Computed privately in Alice's and Bob's systems
    if (alice_symmetric_key == bob_symmetric_key){
        printf("%lld and %lld\n", alice_symmetric_key, bob_symmetric_key);
        printf("K1 = K2 = %lld\nHence a symmetric key is generated .\n", alice_symmetric_key, bob_symmetric_key);
    }
    else
        printf("The Keys generated are not symmetric .\nK1 = %lld K2 = %lld\n", alice_symmetric_key, bob_symmetric_key);

        printf("\n\nPress Enter to see the secure communication taking place\n");
        
        fflush(stdin);
        getchar();
        clear_console();
        
        print_header("Secure Communication in Progress...");
        Sleep(2000);
        printf("\nAlice started encrypting the file with her symmetric copy of key\n");
        Sleep(1000);
        print_info("Encrypting");
        for(int i=0;i<5;i++)
        {
            Sleep(500);
            print_info(".");
        }
        printf(".\n");
        encrypt_file("message.txt", alice_symmetric_key);
        Sleep(2000);
        printf("\nEncrypted file being sent to Bob\n");
         print_info("Sending");
        for(int i=0;i<5;i++)
        {
            Sleep(500);
            print_info(".");
        }
        printf(".\n");
        Sleep(2000);
        printf("File sent successfully\n");
        Sleep(2000);
        printf("\nBob received the file\n");
        Sleep(2000);
        printf("\nBob started decrypting the file with his symmetric copy of key\n");
        Sleep(1000);
         print_info("Decrypting");
        for(int i=0;i<5;i++)
        {
            Sleep(500);
             print_info(".");
        }
        printf(".\n");
        decrypt_file("encrypted.txt", bob_symmetric_key);
    
        Sleep(2000);
       print_header("Secure Communication Completed Successfully");
        Sleep(2000);
         print_info("Exiting...");




    return 0;
}
