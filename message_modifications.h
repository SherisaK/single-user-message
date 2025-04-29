#ifndef PROCESSING_H_
#define PROCESSING_H_

char *redact(char *message, char *list);
char *encrypt_columnar(char *message, char *key);
char *decrypt_columnar(char *message, char *key);

#endif
