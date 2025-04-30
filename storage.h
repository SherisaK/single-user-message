// This is the header file for the storage functions.
// This file defines the message structure and declares the functions for saving and loading
//   messages from the file.

#ifndef FILE_IO_H_
#define FILE_IO_H_

#define MAX_MESSAGES 20    // Maximum number of messages that can be saved.
#define MAX_TITLE_LENGTH 100  // Maximum length of the message title.
#define MAX_MESSAGE_LENGTH 256 // Maximum length of the message content.


// Message Structure to hold a single message.
typedef struct {
    int id;                       // Unique identifier for the message
    char title[MAX_TITLE_LENGTH]; // Title of the message
    char content[MAX_MESSAGE_LENGTH]; // Message content
} Message;

// Reads the entire content of a file into an allocated string.

// A pointer to an allocated string containing the file's content,
//     or NULL if an error occurs (e.g., file not found, memory allocation error).
// The caller is responsible for freeing the allocated memory.
 char *read_string(char *filename);

// Writes a string to a file, overwriting any existing content.

// Errors (e.g., file open error) are typically handled internally
//     (e.g., by printing an error message to stderr).
void write_string(char *filename, char *message);

// Loads message structures from a file into an array.
    
// The function modifies the 'messages' array and the integer.
//     pointed to by 'count'.  
void all_messages(Message messages[], int *count, const char *filename);


// Saves an array of message structures to a file.
void save_messages(Message messages[], int count, const char *filename);

#endif
