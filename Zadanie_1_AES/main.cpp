#include <iostream>
#include "AES.h"

int main() {

    const unsigned char plainText[] = { 0x32, 0x43, 0xf6, 0xa8,
                                  0x88, 0x5a, 0x30, 0x8d,
                                  0x31, 0x31, 0x98, 0xa2,
                                  0xe0, 0x37, 0x07, 0x34 };

    const unsigned char key[] =    { 0x2b, 0x7e, 0x15, 0x16,
                               0x28, 0xae, 0xd2, 0xa6,
                               0xab, 0xf7, 0x15, 0x88,
                               0x09, 0xcf, 0x4f, 0x3c};

    const unsigned char result[] = {0x39, 0x02, 0xdc, 0x19,
                              0x25, 0xdc, 0x11, 0x6a,
                              0x84, 0x09, 0x85, 0x0b,
                              0x1d, 0xfb, 0x97, 0x32 };

    AES aes(4, 4, 10);
    aes.Encrypt(plainText,key);
    printf("\nZakonczono szyfrownie.\n");
    aes.Decrypt(result, key);


    return 0;
}
