#ifndef FILE_IO_H_
#define FILE_IO_H_

#define MAX_MESSAGES 100
#define MAX_TITLE_LENGTH 100
#define MAX_MESSAGE_LENGTH 256

typedef struct {
    int id;
    char title[MAX_TITLE_LENGTH];
    char content[MAX_MESSAGE_LENGTH];
} Message;

char *read_string(char *filename);
void write_string(char *filename, char *message);
void all_messages(Message messages[], int *count, const char *filename);
void save_messages(Message messages[], int count, const char *filename);

#endif
