// This is the header file for the storage functions.
// This file, defines the message structure and declares the various functions for saving and loading 
//    messages from the file.

#ifndef FILE_IO_H_
#define FILE_IO_H_

#define MAX_MESSAGES 20 // Maximum number of messgaes that can be saved.
#define MAX_TITLE_LENGTH 100 // Maximum length of the message title.
#define MAX_MESSAGE_LENGTH 256 // Maximum length of the message content.

typedef struct {
    int id; // The unique identifier for the messages stored chronologically.
    char title[MAX_TITLE_LENGTH]; // Title of the message.
    char content[MAX_MESSAGE_LENGTH]; // Message Content.
} Message;

// A function that akes in a filename and returns the contents as a string.
char *read_string(char *filename);
// A function that takes in a filename and the contents as a string and writes 
//     the contents to the file.
void write_string(char *filename, char *message);
// A function that load the message structure from a file into an array.
void all_messages(Message messages[], int *count, const char *filename);
// A function that saves an array of a messgae to a file.
void save_messages(Message messages[], int count, const char *filename);

#endif
