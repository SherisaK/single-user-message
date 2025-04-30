// This file contains the main function and the user interface 
// for the message/note storing application. It handles user input,
// displays the menu, and calls the appropriate functions based on
// the user's selection.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"
#include "message_modification.h"

#define MAX_MESSAGES 200
#define MAX_TITLE_LENGTH 100
#define MAX_MESSAGE_LENGTH 256


// Function to display the main menu.
// The user gets the options of how they want to navigate the application.
void displayMenu() {
    printf("\n===Message Storage Application===\n");
    printf("1. View all notes/messages stored.\n");
    printf("2. Search to view a note/message stored.\n");
    printf("3. Delete a note/message stored.\n");
    printf("4. Store a new/message.\n");
    printf("5. Search for a note/message using a word/phrase.\n");
    printf("6. Modify an existing stored message.\n");
    printf("0. Exit the Application\n");
    printf("\nPlease enter your choice:\n");
}

// Function to view all messages. Displays the Message ID, Title and the message,
// otherwise, the user gets promted that there is no messages stored.
void view_all_messages(Message messages[], int count) {
    if (count == 0) {
        printf("\nNo notes/messages stored.\n");
          return;
      }
      printf("\n---All Notes/Messages Stored ---\n");
        for (int i = 0; i < count; i++) {
            printf("ID: %d, Title: %s, Preview of Note/Message: %.50s...\n", messages[i].id, messages[i].title, messages[i].content);
        }
    }

// Function to search for a message by ID or title only if there is stored messages.
void search_message(Message messages[], int count) {
    if (count == 0) {
        printf("\nNo notes/messages stored.\n");
        return;
    }

    int search_by_id;
    char search_by_title[MAX_TITLE_LENGTH];
    int found = 0;
    int search_choice;
    
    //Prompts the user to search using ID or Title.
    printf("\nHow do you want to search for the note/message?\n");
    printf("\n1. Search by ID.\n");
    printf("2. Seacrh by Title.\n");
    printf("\nPlease enter your choice:\n");
    scanf("%d", &search_choice);
    getchar(); 


    //Search by ID.
    if (search_choice == 1) {
        printf("\nPlease enter the message ID:\n");
        scanf("%d", &search_by_id);
        getchar(); 
        for (int i = 0; i < count; i++) {
            if (messages[i].id == search_by_id) {
                printf("\n--- Message Found ---\n");
                printf("ID: %d, Title: %s, Content: %s\n", messages[i].id, messages[i].title, messages[i].content);
                found = 1;
                break;
            }
        }
    //Search by Title.
    } else if (search_choice == 2) {
        printf("\nPlease enter message title.\n");
        fgets(search_by_title, MAX_TITLE_LENGTH, stdin);
        search_by_title[strcspn(search_by_title, "\n")] = 0; 
        for (int i = 0; i < count; i++) {
            if (strcmp(messages[i].title, search_by_title) == 0) {
                printf("\n--- Message Found ---\n");
                printf("ID: %d, Title: %s, Content: %s\n", messages[i].id, messages[i].title, messages[i].content);
                found = 1;
                break;
            }
        }
    } else {
        printf("\nXXX Invalid option, please try again. XXX\n");
        return;
    }

    if (!found) {
        printf("\nMessage not found.\n");
    }
}

// Function to delete a message. Once again, first making sure, there are messages stored.
void delete_message(Message messages[], int *count) {
    if (*count == 0) {
        printf("\nNo notes/messages stored.\n");
        return;
    }

    int delete_by_id;
    char delete_by_title[MAX_TITLE_LENGTH];
    int found = 0;
    int delete_choice;
    
    //User gets prompted the options of how they want to search for their message to be deleted.
    printf("How do you want to search for your note/message to be deleted?\n");
    printf("1. Delete by ID.\n");
    printf("2. Delete by Title.\n");
    printf("\nPlease enter your choice:\n");
    scanf("%d", &delete_choice);
    getchar(); 

    //Search and delete by ID.
    if (delete_choice == 1) {
        printf("\nPlease enter the message ID that you wish to delete:\n");
        scanf("%d", &delete_by_id);
        getchar(); 
        for (int i = 0; i < *count; i++) {
            if (messages[i].id == delete_by_id) {
               
                for (int j = i; j < *count - 1; j++) {
                    messages[j] = messages[j + 1];
                }
                (*count)--;
                printf("\nYour note/message has been deleted.\n");
                found = 1;
                break;
            }
        }
    //Search and delete by Title.
    } else if (delete_choice == 2) {
        printf("\nPlease enter the message title to delete: ");
        fgets(delete_by_title, MAX_TITLE_LENGTH, stdin);
        delete_by_title[strcspn(delete_by_title, "\n")] = 0; 
        for (int i = 0; i < *count; i++) {
            if (strcmp(messages[i].title, delete_by_title) == 0) {
                // Shift messages to remove the deleted one
                for (int j = i; j < *count - 1; j++) {
                    messages[j] = messages[j + 1];
                }
                (*count)--;
                printf("\nYour note/message has been deleted.\n");
                found = 1;
                break;
            }
        }
    } else {
        printf("\nXXX Invalid option, please try again. XXX\n");
        return;
    }

    if (!found) {
        printf("\nMessage not found, please try again.\n");
    }
}

// Function to store a new message
// Once again, user gets prompted if there is no messages stored.
// Additionally, the user is given the options of how they want to store this new message.
//       Whether originally, censored or encrypted.
void store_new_message(Message messages[], int *count) {
    if (*count >= MAX_MESSAGES) {
        printf("\nMessage Storage Full. Please delete a message and try again.\n");
        return;
    }

    int storage_choice;
    char message_content[MAX_MESSAGE_LENGTH];

    //Here the User gets prompted the differnt options of how they want to store the messages.
    printf("\nHow do you want to store the message?\n");
    printf("1. Store original note/message.\n");
    printf("2. Censor the note/message before storing.\n");
    printf("3. Encrypt the note/message before storing.\n");
    printf("\nPlease enter your choice:\n");
    scanf("%d", &storage_choice);
    getchar(); 

    messages[*count].id = *count + 1; 

    //This is where the user gets asked to enter the messages' title.
    printf("\nPlease enter the message title:\n");
    fgets(messages[*count].title, MAX_TITLE_LENGTH, stdin);
    messages[*count].title[strcspn(messages[*count].title, "\n")] = 0; 

    //This is where the user gets asked to enter the messages' content.
    printf("\nPlease enter the message content:\n");
    fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
    message_content[strcspn(message_content, "\n")] = 0; 

    if (storage_choice == 1) {
        strcpy(messages[*count].content, message_content);

    } else if (storage_choice == 2) {
        char censored_words[200];
        printf("\nPlease enter the word/s to be censored.\n");
        printf("\nWhere necessary, separate using a comma.\n");
        fgets(censored_words, 200, stdin);
        censored_words[strcspn(censored_words, "\n")] = 0;
        char *censored_message = redact(message_content, censored_words);
        strcpy(messages[*count].content, censored_message);
        free(censored_message); 

    } else if (storage_choice == 3) {
        char key[50];
        printf("\nPlease enter the encryption key:\n");
        fgets(key, 50, stdin);
        key[strcspn(key, "\n")] = 0; 
        char *encrypted_message = encrypt_columnar(message_content, key);
        strcpy(messages[*count].content, encrypted_message);
        free(encrypted_message); 
        
    } else {
        printf("\nXXX Invalid option, please try again. XXX\n");
        return;
    }

    (*count)++;
    printf("\nYour message has been stored successfully.\n");
}

// Function to search for messages by word/phrase.
// Depending on what the user chooses, the application will search for the 
//     message by ID or Title.
void message_search_by_word(Message messages[], int count) {
    if (count == 0) {
        printf("\nNo notes/messages stored.\n");
        return;
    }

    char search_by_word[100];
    int found = 0;

    printf("\nPlease enter word or phrase to search for:\n");
    fgets(search_by_word, 100, stdin);
    search_by_word[strcspn(search_by_word, "\n")] = 0; 

    printf("\n--- Search Results ---\n");
    for (int i = 0; i < count; i++) {
        if (strstr(messages[i].title, search_by_word) != NULL || strstr(messages[i].content, search_by_word) != NULL) {
            printf("ID: %d, Title: %s, Content: %s\n", messages[i].id, messages[i].title, messages[i].content);
            found = 1;
            
            char decrypt_message;
            printf("\nDo you want to decrypt this message? (y/n): ");
            scanf(" %c", &decrypt_message);
            getchar(); 

            if (decrypt_message == 'y' || decrypt_message == 'Y') {
                char key[50];
                printf("\nPlease enter the decryption key:\n");
                fgets(key, 50, stdin);
                key[strcspn(key, "\n")] = 0; 
                char *decrypted_message = decrypt_columnar(messages[i].content, key);
                printf("\nYour message has been decrypted sucessfully: %s\n", decrypted_message);
                free(decrypted_message);
            }
        }
    }

    if (!found) {
        printf("\nNo messages found using that word/phrase.\n");
    }
}

// Function to modify an already stored message.
// Once again, user gets prompted if there is no messages stored.
// Additionally, the user is given the options of how they want to search for the message 
//       they want to modify.
//       Whether by ID or Title.

void modify_message(Message messages[], int count) {
    if (count == 0) {
        printf("\nNo notes/messages stored.\n");
        return;
    }

    int modify_by_id;
    char modify_by_title[MAX_TITLE_LENGTH];
    int found = 0;
    int modify_message;

    printf("\nHow do you want to modify a stored message?\n");
    printf("1. Modify by ID.\n");
    printf("2. MOdify by title.\n");
    printf("\nPlease enter your choice:\n");
    scanf("%d", &modify_message);
    getchar(); 

    // Function to search by ID to modify.
    if (modify_message == 1) {
        printf("\nPlease enter the message ID to modify: ");
        scanf("%d", &modify_by_id);
        getchar(); 
        for (int i = 0; i < count; i++) {
            if (messages[i].id == modify_by_id) {
                found = 1;

                //Option to decrypt the message.
                char decrypt_message;
                printf("\nDo you want to decrypt this message before modifying? (y/n): ");
                scanf(" %c", &decrypt_message);
                getchar(); 

                char message_content[MAX_MESSAGE_LENGTH];
                if (decrypt_message == 'y' || decrypt_message == 'Y') {
                    char key[50];
                    printf("\nPlease enter the decryption key.\n");
                    fgets(key, 50, stdin);
                    key[strcspn(key, "\n")] = 0; 
                    char *decrypted_message = decrypt_columnar(messages[i].content, key);
                    printf("\nCurrent Message Stored: %s\n", decrypted_message);

                    printf("\nPlease enter the new message to be stored.\n");
                    fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
                    message_content[strcspn(message_content, "\n")] = 0; 

                    strcpy(messages[i].content, message_content);
                    free(decrypted_message);
                } else {
                    printf("\nCurrent Message Stored: %s\n", messages[i].content);

                    printf("\nPlease enter the new message to be stored.\n");
                    fgets(message_content, MAX_MESSAGE_LENGTH, stdin);
                    message_content[strcspn(message_content, "\n")] = 0; 

                    strcpy(messages[i].content, message_content);
                }

                printf("\nYour message hase been sucessfully modified.\n");
                break;
            }
        }
    // Function to search by Title to modify.
    } else if (modify_message == 2) {
        printf("\nPlease enter the message Title to modify.\n");
        fgets(modify_by_title, MAX_TITLE_LENGTH, stdin);
        modify_by_title[strcspn(modify_by_title, "\n")] = 0; 
        for (int i = 0; i < count; i++) {
            if (strcmp(messages[i].title, modify_by_title) == 0) {
                found = 1;

                //Option to decrypt the message.
                char decrypt_message;
                printf("\nDo you want to decrypt this message before modifying? (y/n): ");
                scanf(" %c", &decrypt_message);
                getchar(); 

                char tempContent[MAX_MESSAGE_LENGTH];
                if (decrypt_message == 'y' || decrypt_message == 'Y') {
                    char key[50];
                    printf("\nPlease enter the decryption key.\n");
                    fgets(key, 50, stdin);
                    key[strcspn(key, "\n")] = 0; 
                    char *decrypted_message = decrypt_columnar(messages[i].content, key);
                    printf("\nCurrent Message Stored: %s\n", decrypted_message);

                    printf("\nPlease enter the new message to be stored.\n");
                    fgets(tempContent, MAX_MESSAGE_LENGTH, stdin);
                    tempContent[strcspn(tempContent, "\n")] = 0; 

                    strcpy(messages[i].content, tempContent);
                    free(decrypted_message);
                } else {
                    printf("\nCurrent Message Stored: %s\n", messages[i].content);

                    printf("\nPlease enter the new message to be stored.\n");
                    fgets(tempContent, MAX_MESSAGE_LENGTH, stdin);
                    tempContent[strcspn(tempContent, "\n")] = 0; 

                    strcpy(messages[i].content, tempContent);
                }

                printf("\nYour message has been successfully modified.\n");
                break;
            }
        }
    } else {
        printf("\nXXX Invalid option, please try again. XXX\n");
        return;
    }

    if (!found) {
        printf("\nMessage not found, please try again.\n");
    }
}


// This is the main menu function. It has all the potentail options and their corresponding 
// varaibles.
int main() {
    Message messages[MAX_MESSAGES];
    int message_count = 0;
    int choice;
    const char *filename = "Messages_storage.txt"; // File to store messages

    // Loads all the messages from file (Message_storage.txt).
    all_messages(messages, &message_count, filename);

    do {
        displayMenu();
        scanf("%d", &choice);
        getchar(); 

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
            case 0:
                printf("\n You have exitted the program...\n");
                break;
            default:
            printf("\nXXX Invalid option, please try again. XXX\n");
        }

        // Save messages to file after each operation.
        save_messages(messages, message_count, filename);

    } while (choice != 0);

    return 0;
}
