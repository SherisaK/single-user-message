// This file implements the functions for storing
//   and loading messages to/from a file.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h" // Include the header file for this module.

#define MAX_MESSAGES 20    // Maximum number of messages that can be saved.
#define MAX_TITLE_LENGTH 100  // Maximum length of the message title.
#define MAX_MESSAGE_LENGTH 256 // Maximum length of the message content.

// Reads the entire content of a file into a dynamically allocated string.

// A pointer to an allocated string containing the file's content,
//     or NULL if an error occurs (e.g., file not found, memory allocation error).
// The caller is responsible for freeing the allocated memory.
char *read_string(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL; // Return NULL on error.
    }

    // Determine the file size.
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET); // Reset file pointer to the beginning.

    // Allocate memory for the file content + null terminator.
    char *buffer = (char *)malloc(length + 1);
    if (buffer == NULL) {
        fclose(file);
        return NULL; // Return NULL on error.
    }

    // Read the entire file content into the buffer.
    fread(buffer, 1, length, file);
    buffer[length] = '\0'; // Null-terminate the string.

    fclose(file);
    return buffer; // Return the allocated buffer.
}


// Writes a string to a file, overwriting any existing content.
void write_string(char *filename, char *message) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("\nXXX Error opening file. XXX\n"); // Use perror for better error reporting.
        return;
    }

    // Write the string to the file.
    fprintf(file, "%s", message);

    fclose(file);
}


// Loads message structures from a file into an array.

// The function modifies the 'messages' array and the integer
//     pointed to by 'count'.

void all_messages(Message messages[], int *count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        *count = 0; // Set count to 0 if the file doesn't exist or can't be opened.
        return;
    }

    *count = 0; // Initialize message count.
    // Read messages from the file.
    while (fscanf(file, "%d\n", &messages[*count].id) == 1 &&
           fgets(messages[*count].title, MAX_TITLE_LENGTH, file) != NULL &&
           fgets(messages[*count].content, MAX_MESSAGE_LENGTH, file) != NULL) {

        // Remove trailing newlines from title and content.
        messages[*count].title[strcspn(messages[*count].title, "\n")] = '\0';
        messages[*count].content[strcspn(messages[*count].content, "\n")] = '\0';
        (*count)++; // Increment the message count.

        // Check for maximum message limit.
        if (*count >= MAX_MESSAGES) {
            printf("\n!! Warning: Maximum messages reached while loading from file. !!\n");
            break; // Stop reading messages.
        }
    }

    fclose(file);
}


// Saves an array of message structures to a file.
void save_messages(Message messages[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("\nXXX Error opening file. XXX\n"); // Used perror for better error reporting.
        return;
    }

    // Write each message to the file.
    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n%s\n%s\n", messages[i].id, messages[i].title, messages[i].content);
    }

    fclose(file);
}
