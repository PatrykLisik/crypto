#include <iostream>
#include <cstdio>
#include <vector>
#include <cstring>
#include "AES.h"

#define BUFOR_SIZE  (16)

int fromhex(char val) {
    if (val >= '0'  && val <= '9') {
        return val - '0';
    }
    if (val >= 'a' && val <= 'f') {
        return val - 'a' + 10;
    }
    printf("Zmyslny komunikat");
    exit(8);
}

int main(int argc, char* argv[]) {

    std::string inputFileNameString;
    std::string outputFileNameString;
    std::string keyString;
    char* inputFileName;
    char* outputFileName;
    FILE* inputFile;
    FILE* ouputFile;
    std::vector<unsigned char> bufor(BUFOR_SIZE);
    char choice;
    unsigned char key[16];

    if ((argc != 4) && (argc != 5)) {
        printf ("Niepoprawne argumenty programu.\n");
        printf ("1. 'c' lub 'd' - wybor czy bedziemy szyfrowac czy deszyfrowac tekst,\n");
        printf ("2. plik zrodlowy,\n");
        printf ("3. plik docelowy,\n");
        exit(-9);
    }
    else {
        choice = argv[1][0];
        inputFileName = argv[2];
        //std::strcpy(inputFileName, inputFileNameString.c_str());
        outputFileName = argv[3];
        //std::strcpy(outputFileName, outputFileNameString.c_str());
        if (argc == 5) {
            char *byte = argv[4]; //poczatek stringu
            for (int i = 0; i < 16; i++) {
                key[i] = fromhex(*byte) * 16 + fromhex(*(byte + 1));
                byte += 2;
            }
        }
        else {
            unsigned char const defkey[] = { 0x2b, 0x7e, 0x15, 0x16,
                                             0x28, 0xae, 0xd2, 0xa6,
                                             0xab, 0xf7, 0x15, 0x88,
                                             0x09, 0xcf, 0x4f, 0x3c};
            memcpy(key, defkey, 16);
        }
    }



    if (tolower(choice) == 'c' ) {

        inputFile = fopen(inputFileName, "rb");
        ouputFile = fopen(outputFileName, "wb");
        if (!inputFile) {
            printf("Nie mozna przeczytac wskazanego pliku.");
            exit(-2);
        }
        while (!feof(inputFile)) {
            size_t numberOfreadBytes = std::fread(&bufor[0], sizeof bufor[0], bufor.size(), inputFile);

            if (numberOfreadBytes == 0){
                break;
            }
            for (int byte = numberOfreadBytes; byte < BUFOR_SIZE; byte++) {
                bufor[byte] = ' ';
            }

            AES aes(4, 4, 10);
            aes.EncryptBlock(bufor, key, ouputFile);


            for (int i = 0; i < bufor.size(); i++) {
                std::cout << bufor[i];
            }
            std::cout << "\nSTOP\n";
        }

        fclose(inputFile);
        fclose(ouputFile);
    }
    else if (tolower(choice) == 'd') {
        inputFile = fopen(inputFileName, "rb");
        ouputFile = fopen(outputFileName, "wb");
        if (!inputFile) {
            printf("Nie mozna przeczytac wskazanego pliku.");
            exit(-1);
        }
        while (!feof(inputFile)) {
            size_t numberOfreadBytes = std::fread(&bufor[0], sizeof bufor[0], bufor.size(), inputFile);

            if (numberOfreadBytes == 0){
                break;
            }
            for (int byte = numberOfreadBytes; byte < BUFOR_SIZE; byte++) {
                bufor[byte] = ' ';
            }

            AES aes(4, 4, 10);
            aes.DecryptBlock(bufor, key, ouputFile);


            for (int i = 0; i < bufor.size(); i++) {
                std::cout << bufor[i];
            }
            std::cout << "\nSTOP\n";
        }

        fclose(inputFile);
        fclose(ouputFile);
    }
    else {
        printf("\nniepoprawny argument\n");
    }

    return 0;
}
