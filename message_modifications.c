#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "message_modification.h"

// Helper function to trim leading and trailing whitespace from a string
char *delete_white_space(char *str) {
    char *start = str;
    while (isspace((unsigned char)*start)) start++;

    if (*start == 0) // All whitespace, return empty string
        return start;

    char *end = str + strlen(str) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;

    end[1] = '\0'; // Null-terminate after the last non-whitespace char
    return start;
}

// *** HERE IS THE my_strcasestr IMPLEMENTATION ***
char *my_strcasestr(const char *haystack, const char *needle) {
    if (!haystack || !needle) {
        return NULL;
    }

    size_t needle_len = strlen(needle);
    if (needle_len == 0) {
        return (char *)haystack;
    }

    for (const char *h = haystack; *h; ++h) {
        if (strncasecmp(h, needle, needle_len) == 0) {
            return (char *)h;
        }
    }
    return NULL;
}

// Function to redact words from a message
char *redact(char *message, char *list) {
    if (message == NULL || list == NULL) {
        return NULL; 
    }

    char *result = strdup(message); 
    if (result == NULL) {
        return NULL; 
    }

    char *word;
    char *tempList = strdup(list); 
    if (tempList == NULL) {
        free(result);
        return NULL;
    }

    word = strtok(tempList, ","); 
    while (word != NULL) {
        char *trimmed_word = delete_white_space(word); 

        int wordLen = strlen(trimmed_word);
        char *pos = result;

        while ((pos = my_strcasestr(pos, trimmed_word)) != NULL) { 
       
            int prevCharOk = (pos == result || isspace((unsigned char)pos[-1]) || ispunct((unsigned char)pos[-1]));
            int nextCharOk = (pos[wordLen] == '\0' || isspace((unsigned char)pos[wordLen]) || ispunct((unsigned char)pos[wordLen]));

            if (prevCharOk && nextCharOk) {
                for (int i = 0; i < wordLen; i++) {
                    pos[i] = '*';
                }
            }
            pos += 1; 
        }
        word = strtok(NULL, ",");
    }
    free(tempList); 
    return result;
}

// Function to encrypt a message using columnar transposition
char *encrypt_columnar(char *message, char *key) {
    if (message == NULL || key == NULL || strlen(key) == 0) {
        return NULL;
    }

    int messageLen = strlen(message);
    int keyLen = strlen(key);
    int numCols = keyLen;
    int numRows = (messageLen + keyLen - 1) / keyLen;

    char *grid = malloc(numRows * numCols * sizeof(char));
    if (grid == NULL) {
        return NULL;
    }

    int *colOrder = malloc(numCols * sizeof(int)); 
    if (colOrder == NULL) {
        free(grid);
        return NULL;
    }
    for (int i = 0; i < numCols; i++) {
        colOrder[i] = i;
    }

    char *encrypted = malloc(messageLen + numRows + 1);
    if (encrypted == NULL) {
        free(grid);
        free(colOrder);
        return NULL;
    }
    encrypted[0] = '\0';
    int encryptedIndex = 0;

    // Fill the grid
    int messageIndex = 0;
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (messageIndex < messageLen) {
                grid[i * numCols + j] = message[messageIndex++];
            } else {
                grid[i * numCols + j] = '-';
            }
        }
    }

    // Sort column order based on key
    for (int i = 0; i < numCols - 1; i++) {
        for (int j = i + 1; j < numCols - i - 1; j++) {
            if (key[colOrder[j]] > key[colOrder[j + 1]]) {
                int temp = colOrder[j];
                colOrder[j] = colOrder[j + 1];
                colOrder[j + 1] = temp;
            }
        }
    }

    // Read off the encrypted text
    for (int j = 0; j < numCols; j++) {
        for (int i = 0; i < numRows; i++) {
            encrypted[encryptedIndex++] = grid[i * numCols + colOrder[j]];
        }
    }
    encrypted[encryptedIndex] = '\0';

    free(grid);
    free(colOrder); 
    return encrypted;
}

// Function to decrypt a message using columnar transposition
char *decrypt_columnar(char *message, char *key) {
    if (message == NULL || key == NULL || strlen(key) == 0) {
        return NULL;
    }

    int messageLen = strlen(message);
    int keyLen = strlen(key);
    int numCols = keyLen;
    int numRows = messageLen / keyLen;

    char *grid = malloc(numRows * numCols * sizeof(char));
    if (grid == NULL) {
        return NULL;
    }

    int *colOrder = malloc(numCols * sizeof(int));
    if (colOrder == NULL) {
        free(grid);
        return NULL;
    }
    for (int i = 0; i < numCols; i++) {
        colOrder[i] = i;
    }

    int *sortedColOrder = malloc(numCols * sizeof(int)); 
    if (sortedColOrder == NULL) {
        free(grid);
        free(colOrder);
        return NULL;
    }

    char *decrypted = malloc(messageLen + 1);
    if (decrypted == NULL) {
        free(grid);
        free(colOrder);
        free(sortedColOrder);
        return NULL;
    }
    decrypted[0] = '\0';
    int decryptedIndex = 0;

    // Sort column order based on key
    for (int i = 0; i < numCols - 1; i++) {
        for (int j = i + 1; j < numCols - i - 1; j++) {
            if (key[colOrder[j]] > key[colOrder[j + 1]]) {
                int temp = colOrder[j];
                colOrder[j] = colOrder[j + 1];
                colOrder[j + 1] = temp;
            }
        }
    }

    // Fill the grid in encrypted order
    for (int i = 0; i < numCols; i++) {
        sortedColOrder[colOrder[i]] = i; 
    }

    int messageIndex = 0;
    for (int j = 0; j < numCols; j++) {
        for (int i = 0; i < numRows; i++) {
            grid[i * numCols + j] = message[messageIndex++];
        }
    }

    // Read off the decrypted text
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            decrypted[decryptedIndex++] = grid[i * numCols + sortedColOrder[j]];
        }
    }
    decrypted[decryptedIndex] = '\0';

    // Replace dashes with spaces
    for (int i = 0; decrypted[i] != '\0'; i++) {
        if (decrypted[i] == '-') {
            decrypted[i] = ' ';
        }
    }

    free(grid);
    free(colOrder);
    free(sortedColOrder); 
    return decrypted;
}


