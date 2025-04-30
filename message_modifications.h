#ifndef PROCESSING_H_
#define PROCESSING_H_


// Redacts words from a message based on a provided list of words to censor.

// A pointer to a newly allocated string containing the redacted message.
// The function is responsible for freeing the allocated memory.  Returns NULL
//   if an error occurs (e.g., memory allocation failure).
char *redact(char *message, char *list);


// Encrypts a message using the columnar key cipher.

// A pointer to a newly allocated string containing the encrypted message.
// The function is responsible for freeing the allocated memory. Returns NULL
//   if an error occurs (e.g., memory allocation failure, invalid key).
char *encrypt_columnar(char *message, char *key);


// Decrypts a message using the columnar transposition cipher.

// A pointer to a newly allocated string containing the decrypted message.
// The function is responsible for freeing the allocated memory.  Returns NULL
//   if an error occurs (e.g., memory allocation failure, invalid key).
char *decrypt_columnar(char *message, char *key);

#endif // PROCESSING_H_
