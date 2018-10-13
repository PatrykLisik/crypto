#include "AES.h"


AES::AES(int word, int N, int round) : word(word), N(N), round(round) {
    //Ta funkcja, celowo została pozostawiona pusta.
}

void AES::KeyExpansion(unsigned char* key, unsigned char* keysForAllRounds) {

    unsigned char *rcon = new unsigned char[4];
    unsigned char *W_i_minus_N = new unsigned char[4];
    unsigned char *W_i_minus_1 = new unsigned char[4];

    int i = 0;
    int allBytesInKeysForAllRounds = word * N * (round + 1);

    for (i = 0; i < N * word; i++) {
        keysForAllRounds[i] = key[i];
    }

    i = N * word;

    while (i < allBytesInKeysForAllRounds) {

        W_i_minus_N[0] = keysForAllRounds[i - N * word + 0];
        W_i_minus_N[1] = keysForAllRounds[i - N * word + 1];
        W_i_minus_N[2] = keysForAllRounds[i - N * word + 2];
        W_i_minus_N[3] = keysForAllRounds[i - N * word + 3];

        W_i_minus_1[0] = keysForAllRounds[i - N + 0];
        W_i_minus_1[1] = keysForAllRounds[i - N + 1];
        W_i_minus_1[2] = keysForAllRounds[i - N + 2];
        W_i_minus_1[3] = keysForAllRounds[i - N + 3];

        if (i % (N * word) == 0) {
            if (i >= N and (i % N == 0)) {

                RotWord(W_i_minus_1);
                SubWord(W_i_minus_1);
                RoundConstant(rcon, i / (N * word));
                XORWord(W_i_minus_N, W_i_minus_1);
                XORWord(rcon, W_i_minus_1);

                //Result is in W_i_minus_1
            } else if (i >= N and N > 6 and (i % N == 4)) {
                SubWord(W_i_minus_N);
                XORWord(W_i_minus_N, W_i_minus_1);
            } else {
                XORWord(W_i_minus_N, W_i_minus_1);
            }
        }
        else
        {
            XORWord(W_i_minus_N, W_i_minus_1);
        }

        keysForAllRounds[i + 0] = W_i_minus_1[0];
        keysForAllRounds[i + 1] = W_i_minus_1[1];
        keysForAllRounds[i + 2] = W_i_minus_1[2];
        keysForAllRounds[i + 3] = W_i_minus_1[3];
        i += 4;
    }

    delete []rcon;
    delete []W_i_minus_1;
    delete []W_i_minus_N;
}

void AES::RotWord(unsigned char* word) {
    unsigned char tempWord = word[0];
    word[0] = word[1];
    word[1] = word[2];
    word[2] = word[3];
    word[3] = tempWord;
}

void AES::SubWord(unsigned char *word) {
    int i;
    for (i = 0; i < 4; i++)
    {
        word[i] = SBox[word[i] / 16][word[i] % 16];
    }
}

void AES::RoundConstant(unsigned char *rcon, int currentRound) {
    unsigned char rc = 0x01;

    for (int k = 1; k <= currentRound; k++) {
        if (k == 1) {
            rc = 0x01;
        }
        else if (k > 1 and rc < 0x80) {
            rc *= 2;
        }
        else if (k > 1 and rc >= 0x80) {
            rc = (unsigned char)((2 * rc) ^ 0x11B);
        }
        else {
            exit(-5);
        }
    }

    rcon[0] = rc;
    rcon[1] = 0x00;
    rcon[2] = 0x00;
    rcon[3] = 0x00;
}

void AES::XORWord(unsigned char *xoredArray, unsigned char *destination) {

    for (int i = 0; i < 4; i++) {
        destination[i] ^= xoredArray[i];
    }
}

void AES::SubBytes(unsigned char **state) {

    unsigned char valOnPostion;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < word; j++) {
            valOnPostion = state[i][j];
            state[i][j] = SBox[valOnPostion / 16][valOnPostion % 16];
        }
    }
}

void AES::InvSubBytes(unsigned char **state) {

    unsigned char valOnPostion;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < word; j++) {
            valOnPostion = state[i][j];
            state[i][j] = InvSBox[valOnPostion / 16][valOnPostion % 16];
        }
    }
}

void AES::ShiftRows(unsigned char **state) {

    //ShiftRow(state, 0, 0);
    ShiftRow(state, 1, 1);
    ShiftRow(state, 2, 2);
    ShiftRow(state, 3, 3);

}

void AES::ShiftRow(unsigned char **state, int row, int fieldsNumber) {

    unsigned char temp;
    for (int i = 0; i < fieldsNumber; i++) {
        temp = state[row][0];
        for (int j = 0; j < (word - 1); j++) {
            state[row][j] = state[row][j + 1];
        }
        state[row][word - 1] = temp;
    }

}

void AES::InvShiftRows(unsigned char **state) {

    //InvShiftRow(state, 0, word - 0);
    ShiftRow(state, 1, word - 1);
    ShiftRow(state, 2, word - 2);
    ShiftRow(state, 3, word - 3);

}

void AES::MixColumns(unsigned char **state) {

    RotateState(state);
    MultiplyGaloisFieldByWord(state[0]);
    MultiplyGaloisFieldByWord(state[1]);
    MultiplyGaloisFieldByWord(state[2]);
    MultiplyGaloisFieldByWord(state[3]);
    RotateState(state);
}


void AES::MultiplyGaloisFieldByWord(unsigned char *word) {

    int cols = 4;
    int rows = 4;
    unsigned char result;
    unsigned char temp[rows];

    for (int i = 0; i < rows; i++) {

        result = 0;
        for (int j = 0; j < cols; j++) {
            result ^= MultiplyModulo(GaloisField[i][j], word[j]);
        }
        temp[i] = result;
    }

    for (int k = 0; k < rows; k++) {
        word[k] = temp[k];
    }
}

unsigned char AES::MultiplyModulo(const unsigned char field, const unsigned char val) {
    unsigned short int tmp;
    switch (field) {
        case 1:
            return val;
        case 2:
            tmp = (unsigned short int)2 * (unsigned short int)val;
            if (tmp >= 0x100) {
                // jest przeniesienie
                tmp = (unsigned short)(tmp % 0x100);
                tmp ^= 0x1b;
                return (unsigned char)tmp;
            }
            else {
                return (unsigned char)tmp;
            }
        case 3:
            tmp = MultiplyModulo(2,val);
            tmp ^= val;
            return (unsigned char)tmp;
        default:
            exit(-7);
    }
}

void AES::InvMixColumns(unsigned char **state) {

    MixColumns(state);

    //TODO
}

void AES::AddRoundKey(unsigned char **state, unsigned char* roundKey) {

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < word; j++) {
            state[i][j] ^= roundKey[i + 4 * j];
        }
    }
}

void AES::setCurrentRoundKey(unsigned char *currentRoundKey, unsigned char *keysForAllRounds, int currentRound) {

    for (int i = 0; i < N * word; i++) {
        currentRoundKey[i] = keysForAllRounds[(currentRound * word * N) + i];
    }
}

void AES::InvAddRoundKey(unsigned char **state, unsigned char* roundKey) {

    AddRoundKey(state, roundKey);
}

void AES::printState(unsigned char **state) {

    for (int i = 0; i < word; i++) {
        for (int j = 0; j < N; j++) {
            printf("0x%02x ", state[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

void AES::RotateState(unsigned char **state) {

    unsigned char temp[word][N];
    for (int i = 0; i < word; i++) {
        for (int j = 0; j < N; j++) {
            temp[i][j] = state[j][i];
        }
    }

    for (int i = 0; i < word; i++) {
        for (int j = 0; j < N; j++) {
            state[j][i] = temp[j][i];
        }
    }
}

void AES::Encrypt(unsigned char* plainText, unsigned char* key) {

    unsigned char *keysForAllRounds = new unsigned char[N * word * (round + 1)];
    KeyExpansion(key, keysForAllRounds);

    unsigned char **state = new unsigned char *[N];

    for (int i = 0; i < N; i++) {
        state[i] = new unsigned char [word];
    }

    printf("Encrypt::Input\n");
    for(int j = 0; j < N; j++) {
        for (int k = 0; k < word; k++) {
            state[k][j] = plainText[k + j * N];
            printf("0x%02x ", state[k][j]);
        }
        printf("\n");
    }
    printf("\n");

    int currentRound = 0;
    unsigned char currentRoundKey[N * word];

    setCurrentRoundKey(currentRoundKey, keysForAllRounds, currentRound);
    AddRoundKey(state, currentRoundKey);
    printf("AddRoundKey, round %d: \n", currentRound);
    printState(state);

    currentRound++;

    while (currentRound <= (round - 1)) {
        SubBytes(state);
        printf("SubBytes, round %d: \n", currentRound);
        printState(state);

        ShiftRows(state);
        printf("ShiftRows, round %d: \n", currentRound);
        printState(state);

        MixColumns(state);
        printf("MixColumns, round %d: \n", currentRound);
        printState(state);

        setCurrentRoundKey(currentRoundKey, keysForAllRounds, currentRound);
        AddRoundKey(state, currentRoundKey);
        printf("AddRoundKey, round %d: \n", currentRound);
        printState(state);


        currentRound++;
    }

    printf("SubBytes, round %d: \n", currentRound);
    SubBytes(state);
    printState(state);

    printf("ShiftRows, round %d: \n", currentRound);
    ShiftRows(state);
    printState(state);

    printf("AddRoundKey, round %d: \n", currentRound);
    setCurrentRoundKey(currentRoundKey, keysForAllRounds, currentRound);
    AddRoundKey(state, currentRoundKey);
    printState(state);

    //printf("\n");
    //printf("\n");
    //printf("\nKeys:\n");

    //for (int i = 0; i < word * N * (round + 1); i++) {
    //    printf("0x%02x ", keysForAllRounds[i]);
    //    if (i > 1 and ((i % word) == (word - 1))) {
    //        printf("\n");
    //    }
    //    if (i > 1 and ((i % 16) == 15)) {
    //       printf("\n");
    //    }
    //}

    printf("\nEncrypt::Output: \n");
    printState(state);

    for (int i = 0; i < N; i++) {
        delete[] state[i];
    }
    delete[] state;
    delete[] keysForAllRounds;
}



void AES::Decrypt(unsigned char *cipherText, unsigned char *key) {

    unsigned char *keysForAllRounds = new unsigned char[N * word * (round + 1)];
    KeyExpansion(key, keysForAllRounds);

    unsigned char **state = new unsigned char *[N];

    for (int i = 0; i < N; i++) {
        state[i] = new unsigned char [word];
    }

    printf("Decrypt::Input\n");
    for(int j = 0; j < N; j++) {
        for (int k = 0; k < word; k++) {
            state[j][k] = cipherText[k + j * N];
        }
    }
    printState(state);

    int currentRound = round;
    unsigned char currentRoundKey[N * word];

    printf("AddRoundKey, round %d: \n", currentRound);
    setCurrentRoundKey(currentRoundKey, keysForAllRounds, currentRound);
    InvAddRoundKey(state, currentRoundKey);
    printState(state);

    printf("ShiftRows, round %d: \n", currentRound);
    InvShiftRows(state);
    printState(state);

    printf("SubBytes, round %d: \n", currentRound);
    InvSubBytes(state);
    printState(state);

    currentRound--;

    while ((currentRound - 1) >= 1) {

        setCurrentRoundKey(currentRoundKey, keysForAllRounds, currentRound);
        InvAddRoundKey(state, currentRoundKey);
        printf("AddRoundKey, round %d: \n", currentRound);
        printState(state);

        InvMixColumns(state);
        printf("MixColumns, round %d: \n", currentRound);
        printState(state);

        InvShiftRows(state);
        printf("ShiftRows, round %d: \n", currentRound);
        printState(state);

        InvSubBytes(state);
        printf("SubBytes, round %d: \n", currentRound);
        printState(state);

        currentRound--;
    }
/*
    setCurrentRoundKey(currentRoundKey, keysForAllRounds, currentRound);
    AddRoundKey(state, currentRoundKey);
    printf("AddRoundKey, round %d: \n", currentRound);
    printState(state);


    printf("\n");
    printf("\n");
    printf("\nKeys:\n");

    for (int i = 0; i < word * N * (round + 1); i++) {
        printf("0x%02x ", keysForAllRounds[i]);
        if (i > 1 and ((i % word) == (word - 1))) {
            printf("\n");
        }
        if (i > 1 and ((i % 16) == 15)) {
            printf("\n");
        }
    }

    printf("\nDecrypt:: Output: \n");
    printState(state);

    for (int i = 0; i < N; i++) {
        delete[] state[i];
    }
     */
    delete[] state;
    delete[] keysForAllRounds;
}
