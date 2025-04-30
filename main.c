// This file contains the main function and the user interface
// for the message/note storing application. It handles user input,
// displays the menu, and calls the appropriate functions based on
// the user's selection.


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"       // Include the header for storage functions.
#include "message_modification.h" // Include the header for message modification functions.  You may need to create this.

#define MAX_MESSAGES 20    // Maximum number of messages that can be saved.
#define MAX_TITLE_LENGTH 100  // Maximum length of the message title.
#define MAX_MESSAGE_LENGTH 256 // Maximum length of the message content.


// Function displays the main menu to the user, showing the available options.
void displayMenu() {
    printf("\n=== Message Storage Application ===\n");
    printf("1. View all notes/messages stored.\n");
    printf("2. Search to view a note/message stored.\n");
    printf("3. Delete a note/message stored.\n");
    printf("4. Store a new/message.\n");
    printf("5. Search for a note/message using a word/phrase.\n");
    printf("6. Modify an existing stored message.\n");
    printf("0. Exit the Application\n");
    printf("\n Please enter your choice: \n");
}


// Displays all stored messages (ID, title, and a preview of the content).
void view_all_messages(Message messages[], int count) {
    if (count == 0) {
        printf("\n No notes/messages stored. \n");
        return;
    }
    printf("\n --- All Notes/Messages Stored --- \n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d, Title: %s, Preview of Note/Message: %.50s...\n",
               messages[i].id, messages[i].title, messages[i].content);
    }
}

// Searches for a message by ID or title.
void search_message(Message messages[], int count) {
    if (count == 0) {
        printf("\n No notes/messages stored. \n");
        return;
    }

    int search_by_id;
    char search_by_title[MAX_TITLE_LENGTH];
    int found = 0;
    int search_choice;

    // Prompt the user to search using ID or Title.
    printf("\n How do you want to search for the note/message? \n");
    printf("\n1. Search by ID.\n");
    printf("2. Search by Title.\n");
    printf("\n Please enter your choice: \n");
    scanf("%d", &search_choice);
    getchar(); // Consume the newline character.

    // Search by ID.
    if (search_choice == 1) {
        printf("\n Please enter the message ID: \n");
        scanf("%d", &search_by_id);
        getchar(); // Consume the newline character.
        for (int i = 0; i < count; i++) {
            if (messages[i].id == search_by_id) {
                printf("\n --- Message Found --- \n");
                printf("ID: %d, Title: %s, Content: %s\n", messages[i].id, messages[i].title, messages[i].content);
                found = 1;
                break;
            }
        }
        // Search by Title.
    } else if (search_choice == 2) {
        printf("\n Please enter message title. \n");
        fgets(search_by_title, MAX_TITLE_LENGTH, stdin);
        search_by_title[strcspn(search_by_title, "\n")] = '\0'; // Remove trailing newline.
        for (int i = 0; i < count; i++) {
            if (strcmp(messages[i].title, search_by_title) == 0) {
                printf("\n --- Message Found --- \n");
                printf("ID: %d, Title: %s, Content: %s\n", messages[i].id, messages[i].title, messages[i].content);
                found = 1;
                break;
            }
        }
    } else {
        printf("\n XXX Invalid option, please try again. XXX \n");
        return;
    }

    if (!found) {
        printf("\n Message not found. \n");
    }
}


// Function deletes a message by ID or title.
void delete_message(Message messages[], int *count) {
    if (*count == 0) {
        printf("\n No notes/messages stored. \n");
        return;
    }

    int delete_by_id;
    char delete_by_title[MAX_TITLE_LENGTH];
    int found = 0;
    int delete_choice;

    // Prompt the user for how they want to search for the message to be deleted.
    printf("\n How do you want to search for your note/message to be deleted? \n");
    printf("1. Delete by ID.\n");
    printf("2. Delete by Title.\n");
    printf("\n Please enter your choice: \n");
    scanf("%d", &delete_choice);
    getchar(); // Consume the newline character.

    // Search and delete by ID.
    if (delete_choice == 1) {
        printf("\n Please enter the message ID that you wish to delete: \n");
        scanf("%d", &delete_by_id);
        getchar(); // Consume the newline character.
        for (int i = 0; i < *count; i++) {
            if (messages[i].id == delete_by_id) {
                // Shift messages to remove the deleted one.
                for (int j = i; j < *count - 1; j++) {
                    messages[j] = messages[j + 1];
                }
                (*count)--; // Decrement the count.
                printf("\n Your note/message has been deleted. \n");
                found = 1;
                break;
            }
        }
        // Search and delete by Title.
    } else if (delete_choice == 2) {
        printf("\n Please enter the message title to delete: \n");
        fgets(delete_by_title, MAX_TITLE_LENGTH, stdin);
        delete_by_title[strcspn(delete_by_title, "\n")] = '\0'; // Remove trailing newline.
        for (int i = 0; i < *count; i++) {
            if (strcmp(messages[i].title, delete_by_title) == 0) {
                // Shift messages to remove the deleted one.
                for (int j = i; j < *count - 1; j++) {
                    messages[j] = messages[j + 1];
                }
                (*count)--; // Decrement the count.
                printf("\n Your note/message has been deleted. \n");
                found = 1;
                break;
            }
        }
    } else {
        printf("\n XXX Invalid option, please try again. XXX \n");
        return;
    }

    if (!found) {
        printf("\n Message not found, please try again. \n");
    }
}


// Stores a new message, with options for original, censored, or encrypted storage.
void store_new_message(Message messages[], int *count) {
    if (*count >= MAX_MESSAGES) {
        printf("\n Message Storage Full. Please delete a message and try again. \n");
        return;
    }

    int storage_choice;
    char message_content[MAX_MESSAGE_LENGTH];
    char censored_words[200]; //increased size.
    char key[50];

    // Prompt the user for how they want to store the message.
    printf("\n How do you want to store the message? \n");
    printf("1. Store original note/message.\n");
    printf("2. Censor the note/message before storing.\n");
    printf("3. Encrypt the note/message before storing.\n");
    printf("\n Please enter your choice: \n");
    scanf("%d", &storage_choice);
    getchar(); // Consume the newline character.

    messages[*count].id = *count + 1; // Assign a unique ID.

    // Get the message title.
    printf("\n Please enter the message title: \n");
    fgets(messages[*count].title, MAX_TITLE_LENGTH, stdin);
    messages[*count].title[strcspn(messages[*count].title, "\n")] = '\0'; // Remove trailing newline.

    // Get the message content.
    printf("\n Please enter the message content: \n");
    fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
    message_content[strcspn(message_content, "\n")] = '\0'; // Remove trailing newline.

    if (storage_choice == 1) {
        // Store the original message.
        strcpy(messages[*count].content, message_content);
    } else if (storage_choice == 2) {
        // Censor the message before storing.
        printf("\n Please enter the word/s to be censored. \n");
        printf("Where necessary, separate using a comma. \n");
        fgets(censored_words, 200, stdin);
        censored_words[strcspn(censored_words, "\n")] = '\0'; // Remove trailing newline.
        char *censored_message = redact(message_content, censored_words); // Use the redact function.
        if (censored_message != NULL)
        {
          strcpy(messages[*count].content, censored_message);
          free(censored_message);
        }
        else
        {
            strcpy(messages[*count].content, message_content);
        }
    } else if (storage_choice == 3) {
        // Encrypt the message before storing.
        printf("\n Please enter the encryption key: \n");
        fgets(key, 50, stdin);
        key[strcspn(key, "\n")] = '\0'; // Remove trailing newline.
        char *encrypted_message = encrypt_columnar(message_content, key); // Use the encrypt_columnar function.
        if(encrypted_message != NULL)
        {
          strcpy(messages[*count].content, encrypted_message);
          free(encrypted_message);
        }
        else
        {
            strcpy(messages[*count].content, message_content);
        }
    } else {
        printf("\n XXX Invalid option, please try again. XXX \n");
        return;
    }

    (*count)++; // Increment the message count.
    printf("\n Your message has been stored successfully. \n");
}

//Searches for messages containing a specific word or phrase.
void message_search_by_word(Message messages[], int count) {
    if (count == 0) {
        printf("\n No notes/messages stored. \n");
        return;
    }

    char search_by_word[100];
    int found = 0;
    char decrypt_message;
    char key[50];
    char *decrypted_message;

    printf("\n Please enter word or phrase to search for: \n");
    fgets(search_by_word, 100, stdin);
    search_by_word[strcspn(search_by_word, "\n")] = '\0'; // Remove trailing newline

    printf("\n --- Search Results --- \n");
    for (int i = 0; i < count; i++) {
        if (strstr(messages[i].title, search_by_word) != NULL || strstr(messages[i].content, search_by_word) != NULL) {
            printf("ID: %d, Title: %s, Content: %s\n", messages[i].id, messages[i].title, messages[i].content);
            found = 1;

            printf("\n Do you want to decrypt this message? (y/n): \n");
            scanf(" %c", &decrypt_message);
            getchar(); // Consume the newline.

            if (decrypt_message == 'y' || decrypt_message == 'Y') {
                printf("\nPlease enter the decryption key:\n");
                fgets(key, 50, stdin);
                key[strcspn(key, "\n")] = '\0';
                decrypted_message = decrypt_columnar(messages[i].content, key); // Use decrypt function.
                if (decrypted_message != NULL)
                {
                  printf("\n Your message has been decrypted successfully: %s \n", decrypted_message);
                  free(decrypted_message);
                }
                else
                {
                    printf("\n Your message could not be decrypted. \n");
                }
            }
        }
    }

    if (!found) {
        printf("\n No messages found using that word/phrase. \n");
    }
}


// Modifies an existing message by ID or title.
void modify_message(Message messages[], int count) {
    if (count == 0) {
        printf("\n No notes/messages stored. \n");
        return;
    }

    int modify_by_id;
    char modify_by_title[MAX_TITLE_LENGTH];
    int found = 0;
    int modify_choice;
    char message_content[MAX_MESSAGE_LENGTH];
    char key[50];
    char *decrypted_message;

    printf("\n How do you want to modify a stored message? \n");
    printf("1. Modify by ID.\n");
    printf("2. Modify by title.\n");
    printf("\n Please enter your choice: \n");
    scanf("%d", &modify_choice);
    getchar(); // Consume the newline character.

    // Function to search by ID to modify.
    if (modify_choice == 1) {
        printf("\n Please enter the message ID to modify: \n");
        scanf("%d", &modify_by_id);
        getchar(); // Consume the newline character.
        for (int i = 0; i < count; i++) {
            if (messages[i].id == modify_by_id) {
                found = 1;

                // Option to decrypt the message.
                char decrypt_message;
                printf("\n Do you want to decrypt this message before modifying? (y/n): \n");
                scanf(" %c", &decrypt_message);
                getchar(); // Consume the newline.

                if (decrypt_message == 'y' || decrypt_message == 'Y') {
                    printf("\n Please enter the decryption key. \n");
                    fgets(key, 50, stdin);
                    key[strcspn(key, "\n")] = '\0';
                    decrypted_message = decrypt_columnar(messages[i].content, key); // Use decrypt function.
                    if (decrypted_message != NULL)
                    {
                      printf("\nCurrent Message Stored: %s\n", decrypted_message);

                      printf("\n Please enter the new message to be stored. \n");
                      fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
                      message_content[strcspn(message_content, "\n")] = '\0';

                      strcpy(messages[i].content, message_content);
                      free(decrypted_message);
                    }
                    else
                    {
                         printf("\n Current Message Stored: %s \n", messages[i].content);

                      printf("\n Please enter the new message to be stored. \n");
                      fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
                      message_content[strcspn(message_content, "\n")] = '\0';

                      strcpy(messages[i].content, message_content);
                    }
                } else {
                    printf("\n Current Message Stored: %s \n", messages[i].content);

                    printf("\n Please enter the new message to be stored. \n");
                    fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
                    message_content[strcspn(message_content, "\n")] = '\0';

                    strcpy(messages[i].content, message_content);
                }

                printf("\n Your message has been successfully modified. \n");
                break;
            }
        }
        // Function to search by Title to modify.
    } else if (modify_choice == 2) {
        printf("\n Please enter the message Title to modify. \n");
        fgets(modify_by_title, MAX_TITLE_LENGTH, stdin);
        modify_by_title[strcspn(modify_by_title, "\n")] = '\0'; // Remove trailing newline.
        for (int i = 0; i < count; i++) {
            if (strcmp(messages[i].title, modify_by_title) == 0) {
                found = 1;

                // Option to decrypt the message.
                char decrypt_message;
                printf("\n Do you want to decrypt this message before modifying? (y/n): \n");
                scanf(" %c", &decrypt_message);
                getchar(); // Consume the newline.

                if (decrypt_message == 'y' || decrypt_message == 'Y') {
                    printf("\n Please enter the decryption key. \n");
                    fgets(key, 50, stdin);
                    key[strcspn(key, "\n")] = '\0';
                    decrypted_message = decrypt_columnar(messages[i].content, key);  // Use decrypt function.
                    if(decrypted_message != NULL)
                    {
                      printf("\n Current Message Stored: %s \n", decrypted_message);

                      printf("\n Please enter the new message to be stored. \n");
                      fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
                      message_content[strcspn(message_content, "\n")] = '\0';

                      strcpy(messages[i].content, message_content);
                      free(decrypted_message);
                    }
                    else
                    {
                      printf("\n Current Message Stored: %s \n", messages[i].content);

                      printf("\n Please enter the new message to be stored. \n");
                      fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
                      message_content[strcspn(message_content, "\n")] = '\0';

                      strcpy(messages[i].content, message_content);
                    }
                } else {
                    printf("\n Current Message Stored: %s \n", messages[i].content);

                    printf("\nPlease enter the new message to be stored.\n");
                    fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
                    message_content[strcspn(message_content, "\n")] = '\0';

                    strcpy(messages[i].content, message_content);
                }

                printf("\n Your message has been successfully modified. \n");
                break;
            }
        }
    } else {
        printf("\n XXX Invalid option, please try again. XXX \n");
        return;
    }

    if (!found) {
        printf("\n Message not found, please try again. \n");
    }
}


// The main function of the application.  Handles the main menu loop,
//     user input, and calls the appropriate functions.
int main() {
    Message messages[MAX_MESSAGES];
    int message_count = 0;
    int choice;
    const char *filename = "Messages_storage.txt"; // File to store messages.

    // Loads all the messages from file (Messages_storage.txt).
    all_messages(messages, &message_count, filename);

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // Consume the newline character.

        switch (choice) {
            case 1:
                view_all_messages(messages, message_count);
                break;
            case 2:
                search_message(messages, message_count);
                break;
            case 3:
                delete_message(messages, &message_count);
                break;
            case 4:
                store_new_message(messages, &message_count);
                break;
            case 5:
                message_search_by_word(messages, message_count);
                break;
            case 6:
                modify_message(messages, message_count);
                break;
            case 0:
                printf("\n You have exited the program... \n");
                break;
            default:
                printf("\n XXX Invalid option, please try again. XXX \n");
        }

        // Save messages to file after each operation.
        save_messages(messages, message_count, filename);

    } while (choice != 0);

    return 0;
}
