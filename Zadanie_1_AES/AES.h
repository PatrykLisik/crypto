#ifndef AES_AES_H
#define AES_AES_H

#include <iostream>
#include <vector>


class AES {
private:
    int word;       //Liczba bajtów w kolumnie
    int N;          //Długość klucza
    int round;      //Runda szyfrownania
public:
    AES(int word, int N, int round);

private:
    void KeyExpansion(unsigned char* key, unsigned char* keysForAllRounds);
    void RotWord(unsigned char* word);
    void SubWord(unsigned char *word);
    void RoundConstant(unsigned char *rcon, int currentRound);

    void XORWord(unsigned char *copy, unsigned char *column);

    void SubBytes(unsigned char **state);
    void InvSubBytes(unsigned char **state);

    void ShiftRows(unsigned char **state);
    void ShiftRow(unsigned char **state, int row, int fieldsNumber);
    void InvShiftRows(unsigned char **state);

    void MixColumns(unsigned char **state);
    void InvMixColumns(unsigned char **state);
    unsigned char L(unsigned char val);
    unsigned char E(unsigned char a, unsigned char b);


    void AddRoundKey(unsigned char **state, unsigned char* roundKey);
    void setCurrentRoundKey(unsigned char *currentRoundKey, unsigned char *keyForAllRoundsm, int currentRound);
    void InvAddRoundKey(unsigned char **state, unsigned char* roundKey);

    void printState(unsigned char **state);

    void RotateState(unsigned char **state);

public:
    void EncryptBlock(std::vector<unsigned char> plainText, unsigned char *key, FILE* ouputFile);
    void DecryptBlock(std::vector<unsigned char> cipherText, unsigned char *key, FILE* ouputFile);

};





#endif //AES_AES_H
