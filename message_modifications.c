// File that inclues the implementations of the functions that modify a message.
// This file contains functions for redacting (censoring), encrypting, and
// decrypting messages. It also includes a helper function for trimming
//     whitespace from strings (when a message is deleted or shortened).

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "message_modification.h"

// Helper function to trim leading and trailing whitespace from a string.

// Repeats from the beginning of the string to find the first non-whitespace
//     character.
// Repeats from the end of the string to find the last non-whitespace character.
// Null-terminates the string after the last non-whitespace character.
// Handles any cases where the string is entirely whitespace.

char *delete_white_space(char *str) {
    char *start = str;

    // Trim leading whitespace
    while (isspace((unsigned char)*start)) start++;

    // Handle empty or all-whitespace string
    if (*start == 0)
        return start;

    char *end = str + strlen(str) - 1;

    // Trim trailing whitespace
    while (end > start && isspace((unsigned char)*end)) end--;

    // Null-terminate the trimmed string
    end[1] = '\0';
    return start;
}

// Performs a case-insensitive search for the first occurrence of a substring
//     within a string.

// Handles NULL input by returning NULL.
// If 'needle' is an empty string, returns 'haystack' (as an empty string
//     is considered to be found at the beginning of any string).
// Repeats through 'haystack', comparing substrings of 'haystack' with
//     'needle' using 'strncasecmp' for case-insensitive comparison.
// Returns a pointer to the matching substring if found, otherwise returns NULL.

char *my_strcasestr(const char *haystack, const char *needle) {
    // Handle NULL input
    if (!haystack || !needle) {
        return NULL;
    }

    size_t needle_len = strlen(needle);

    // Handle empty needle
    if (needle_len == 0) {
        return (char *)haystack;
    }

    // Search for the needle in the haystack
    for (const char *h = haystack; *h; ++h) {
        if (strncasecmp(h, needle, needle_len) == 0) {
            return (char *)h;
        }
    }
    return NULL;
}

// Function to redact words from a message.

// Handles NULL input by returning NULL.
// Creates a copy of the input message ('result') using 'strdup'.
// Creates a temporary copy of the word list ('tempList') using 'strdup' for tokenization.
// Uses 'strtok' to extract individual words from the word list.
// For each word, it searches for case-insensitive occurrences in the message using 'my_strcasestr'.
// If a word is found, it checks if it's a whole word (preceded and followed by whitespace or punctuation).
// If it's a whole word, it replaces the word with asterisks ('*').
// Frees the dynamically allocated memory for 'tempList' and returns the 'result'.

char *redact(char *message, char *list) {
    // Handle NULL input
    if (message == NULL || list == NULL) {
        return NULL;
    }

    // Create a copy of the message to redact.
    char *result = strdup(message);
    if (result == NULL) {
        return NULL; // Memory allocation error.
    }

    char *word;
    // Create a copy of the list for tokenization.
    char *tempList = strdup(list);
    if (tempList == NULL) {
        free(result);
        return NULL; // Memory allocation error.
    }

    // Use '*' instead of the list of words to redact.
    word = strtok(tempList, ",");
    while (word != NULL) {
        char *trimmed_word = delete_white_space(word); // Trim whitespace around the word

        int wordLen = strlen(trimmed_word);
        char *pos = result;

        // Find and redact all occurrences of the word
        while ((pos = my_strcasestr(pos, trimmed_word)) != NULL) {

            // Check if it's a whole word.
            int prevCharOk = (pos == result || isspace((unsigned char)pos[-1]) || ispunct((unsigned char)pos[-1]));
            int nextCharOk = (pos[wordLen] == '\0' || isspace((unsigned char)pos[wordLen]) || ispunct((unsigned char)pos[wordLen]));

            // Redact the word if it's a whole word.
            if (prevCharOk && nextCharOk) {
                for (int i = 0; i < wordLen; i++) {
                    pos[i] = '*';
                }
            }
            pos += 1; // Move past the redacted word to avoid infinite loops.
        }
        word = strtok(NULL, ","); // Get the next word.
    }
    free(tempList); // Free the temporary list.
    return result; // Return the redacted message.
}


// Function to encrypt a message using columnar transposition.

// Handles NULL or empty key input by returning NULL.
// Calculates the number of rows and columns for the encryption grid.
// Allocates memory for the grid ('grid'), the column order ('colOrder'), and the
//     encrypted message ('encrypted').
// Fills the grid with the message characters, padding with '-' if necessary.
// Sorts the column order based on the key.
// Reads the characters from the grid column by column, according to the sorted
//     column order, to construct the encrypted message.
// Null-terminates the encrypted message.
// Frees the allocated memory and returns the encrypted message.
 
char *encrypt_columnar(char *message, char *key) {
    // Handle NULL or empty key.
    if (message == NULL || key == NULL || strlen(key) == 0) {
        return NULL;
    }

    int messageLen = strlen(message);
    int keyLen = strlen(key);
    int numCols = keyLen;
    int numRows = (messageLen + keyLen - 1) / keyLen; // Calculate number of rows.

    // Allocate memory for the grid.
    char *grid = malloc(numRows * numCols * sizeof(char));
    if (grid == NULL) {
        return NULL; // Memory allocation error.
    }

    // Allocate memory for the column order array.
    int *colOrder = malloc(numCols * sizeof(int));
    if (colOrder == NULL) {
        free(grid);
        return NULL; // Memory allocation error.
    }
    for (int i = 0; i < numCols; i++) {
        colOrder[i] = i; // Initialize column order.
    }

    // Allocate memory for the encrypted message.
    char *encrypted = malloc(messageLen + numRows + 1); // Account for potential padding.
    if (encrypted == NULL) {
        free(grid);
        free(colOrder);
        return NULL; // Memory allocation error.
    }
    encrypted[0] = '\0'; // Initialize as an empty string.
    int encryptedIndex = 0;

    // Fill the grid with message characters.
    int messageIndex = 0;
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            if (messageIndex < messageLen) {
                grid[i * numCols + j] = message[messageIndex++];
            } else {
                grid[i * numCols + j] = '-'; // Pad with '-'.
            }
        }
    }

    // Sort column order based on key.
    for (int i = 0; i < numCols - 1; i++) {
        for (int j = i + 1; j < numCols; j++) {
            if (key[colOrder[j]] > key[colOrder[j + 1]]) {
                int temp = colOrder[j];
                colOrder[j] = colOrder[j + 1];
                colOrder[j + 1] = temp;
            }
        }
    }

    // Read off the encrypted text from the grid.
    for (int j = 0; j < numCols; j++) {
        for (int i = 0; i < numRows; i++) {
            encrypted[encryptedIndex++] = grid[i * numCols + colOrder[j]];
        }
    }
    encrypted[encryptedIndex] = '\0'; // Null-terminate the encrypted message.

    // Free allocated memory.
    free(grid);
    free(colOrder);
    return encrypted;
}

// Function to decrypt a message using columnar transposition.

// Handles NULL or empty key input by returning NULL.
// Calculates the number of rows and columns from the encrypted message and key.
// Allocates memory for the grid ('grid'), the column order ('colOrder'), the
//     sorted column order ('sortedColOrder'), and the decrypted message ('decrypted').
// Sorts the column order based on the key (same as in encryption).
// Fills the grid with the encrypted message characters, following the original
//     column order.
// Reads the characters from the grid row by row, using the sorted column order
//     to determine the original column, to construct the decrypted message.
// Null-terminates the decrypted message.
// Replaces any padding characters ('-') with spaces.
// Frees the dynamically allocated memory and returns the decrypted message.

char *decrypt_columnar(char *message, char *key) {
    // Handle NULL or empty key.
    if (message == NULL || key == NULL || strlen(key) == 0) {
        return NULL;
    }

    int messageLen = strlen(message);
    int keyLen = strlen(key);
    int numCols = keyLen;
    int numRows = messageLen / keyLen; // Calculate number of rows.

    // Allocate memory for the grid.
    char *grid = malloc(numRows * numCols * sizeof(char));
    if (grid == NULL) {
        return NULL; // Memory allocation error.
    }

    // Allocate memory for the column order array.
    int *colOrder = malloc(numCols * sizeof(int));
    if (colOrder == NULL) {
        free(grid);
        return NULL; // Memory allocation error.
    }
    for (int i = 0; i < numCols; i++) {
        colOrder[i] = i; // Initialize column order.
    }

    // Allocate memory for the sorted column order array.
    int *sortedColOrder = malloc(numCols * sizeof(int));
    if (sortedColOrder == NULL) {
        free(grid);
        free(colOrder);
        return NULL; // Memory allocation error.
    }

    // Allocate memory for the decrypted message.
    char *decrypted = malloc(messageLen + 1);
    if (decrypted == NULL) {
        free(grid);
        free(colOrder);
        free(sortedColOrder);
        return NULL; // Memory allocation error.
    }
    decrypted[0] = '\0'; // Initialize as an empty string.
    int decryptedIndex = 0;

    // Sort column order based on key (same as in encryption).
    for (int i = 0; i < numCols - 1; i++) {
        for (int j = i + 1; j < numCols; j++) {
            if (key[colOrder[j]] > key[colOrder[j + 1]]) {
                int temp = colOrder[j];
                colOrder[j] = colOrder[j + 1];
                colOrder[j + 1] = temp;
            }
        }
    }

    // Create a mapping from original column order to sorted column order.
    for (int i = 0; i < numCols; i++) {
        sortedColOrder[colOrder[i]] = i;
    }

    // Fill the grid with the encrypted message.
    int messageIndex = 0;
    for (int j = 0; j < numCols; j++) {
        for (int i = 0; i < numRows; i++) {
            grid[i * numCols + j] = message[messageIndex++];
        }
    }

    // Read off the decrypted text from the grid.
    for (int i = 0; i < numRows; i++) {
        for (int j = 0; j < numCols; j++) {
            decrypted[decryptedIndex++] = grid[i * numCols + sortedColOrder[j]];
        }
    }
    decrypted[decryptedIndex] = '\0'; // Null-terminate the decrypted message.

    // Replace padding dashes with spaces (if any)
    for (int i = 0; decrypted[i] != '\0'; i++) {
        if (decrypted[i] == '-') {
            decrypted[i] = ' ';
        }
    }

    // Free allocated memory.
    free(grid);
    free(colOrder);
    free(sortedColOrder);
    return decrypted;
}


