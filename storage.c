#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define MAX_MESSAGES 100
#define MAX_TITLE_LENGTH 100
#define MAX_MESSAGE_LENGTH 256

// Function to read the entire content of a file into a string
char *read_string(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return NULL; 
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *buffer = (char *)malloc(length + 1); 
    if (buffer == NULL) {
        fclose(file);
        return NULL; 
    }

    fread(buffer, 1, length, file);
    buffer[length] = '\0'; 

    fclose(file);
    return buffer;
}

// Function to write a string to a file
void write_string(char *filename, char *message) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("\nXXX Error opening file. XXX\n");
        return;
    }

    fprintf(file, "%s", message);

    fclose(file);
}

// Function to load messages from the file
void all_messages(Message messages[], int *count, const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        *count = 0; // No file, start with empty
        return;
    }

    *count = 0;
    while (fscanf(file, "%d\n", &messages[*count].id) == 1 &&
           fgets(messages[*count].title, MAX_TITLE_LENGTH, file) != NULL &&
           fgets(messages[*count].content, MAX_MESSAGE_LENGTH, file) != NULL) {
        // Remove trailing newlines
        messages[*count].title[strcspn(messages[*count].title, "\n")] = '\0';
        messages[*count].content[strcspn(messages[*count].content, "\n")] = '\0';
        (*count)++;

        if (*count >= MAX_MESSAGES) {
            printf("\n !! Warning: Maximum messages reached while loading from file.!! \n");
            break;
        }
    }

    fclose(file);
}

// Function to save messages to the file
void save_messages(Message messages[], int count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("\nXXX Error opening file. XXX\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "%d\n%s\n%s\n", messages[i].id, messages[i].title, messages[i].content);
    }

    fclose(file);
}
