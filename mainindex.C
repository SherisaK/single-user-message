//This one works, but it is missing alot. Completed it in the other files.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MESSAGES 100
#define MAX_MESSAGE_LENGTH 256

// Structure to hold a message
typedef struct {
    char text[MAX_MESSAGE_LENGTH];
} Message;

// Array to store messages
Message messages[MAX_MESSAGES];
int message_count = 0;

void display_menu();
void view_messages();
void search_message();
void delete_message();
void store_message();
void display_first_menu();
void user_first_input(int choice);
void search_word_phrase ();
void modify_message ();

//Need to add in these functions.
//  char * read_string( char *filename) 
//  void write_string( char *filename, char* message)
//  char * redact( char *message, char *list)
//  char * encrypt_columnar( char *message, char *key)
//  char * decrypt_columnar( char *message, char *key)

int main() {
    int first_choice;

    printf("Welcome to the Message Storage Application!\n");

    do {
        display_first_menu();
        printf("\nPlease choose an option: \n");
        scanf("%d", &first_choice);
        user_first_input(first_choice);
    } while (first_choice != 2); // Continue until user chooses to exit

    return 0;
}

void display_first_menu() {
    printf("\n==== Main Menu ====\n");
    printf("1. Enter the Storage Application.\n");
    printf("2. Exit the Application\n");
}

void user_first_input(int choice) {
    switch (choice) {
        case 1:
            printf("\nYou chose to enter the Storage Application.\n");
            int user_main_choice;
            do {
                display_menu();
                scanf("%d", &user_main_choice);
                while (getchar() != '\n');
                switch (user_main_choice) {
                    case 1:
                        printf("\nYou chose to view all messages/notes.\n");
                        view_messages();
                        break;
                    case 2:
                        printf("\nYou can now search for a message.\n");
                        search_message();
                        break;
                    case 3:
                        printf("\nYou can now delete a message/note.\n");
                        delete_message();
                        break;
                    case 4:
                        printf("\nYou can now store a new message.\n");
                        store_message();
                        break;
                   case 5:
                        printf("\nYou can now search for a message/note using a word/phrase.\n");
                        search_word_phrase();
                        break;
                   case 6:
                        printf("\nYou can now modify a message/note.\n");
                        modify_message();
                        break;
                    case 0:
                        printf("\nYou have exited the application.\n");
                        break;

                    default:
                        printf("\nXX Invalid choice. Please try again. XX\n");
                }
            } while (user_main_choice != 0); // Continue until user chooses to exit

            break;
        case 2:
            printf("\nYou chose to exit the application.\n");
            exit(0);
        default:
            printf("\nXX Invalid choice. Please try again. XX\n");
    }
}

void display_menu() {
    printf("\n==== Message Storage Application ====\n");
    printf("1. View all messages.\n");
    printf("2. Search for a message.\n");
    printf("3. Delete a message.\n");
    printf("4. Store new message.\n");
    printf("0. Exit\n");
    printf("\nPlease enter your choice: \n");
}

void view_messages() {
    //Implement the message id, title and partial message/note to screen for each of the messages.
    if (message_count == 0) {
        printf("\nNo messages stored yet.\n");
        return;
    }
    printf("\n==== Your Messages ====\n");
    for (int i = 0; i < message_count; i++) {
        printf("%d: %s\n", i + 1, messages[i].text);
    }
}

void search_message() {
    //Implement enter the message/note id or title after which the application searches for and once found, displays the message id, title and the full message to screen. If the message/note was encrypted, the user should have the option to decrypt the message/note to view it as plain text.
    if (message_count == 0) {
        printf("\nNo messages to search.\n");
        return;
    }
   char search_term[MAX_MESSAGE_LENGTH];
    printf("\nEnter the text to search for: ");
fgets(search_term, MAX_MESSAGE_LENGTH, stdin);
    search_term[strcspn(search_term, "\n")] = 0; // Remove trailing newline

    int found = 0;
    printf("\n==== Search Results ====\n");
    for (int i = 0; i < message_count; i++) {
        if (strstr(messages[i].text, search_term) != NULL) {
            printf("%d: %s\n", i + 1, messages[i].text);
            found = 1;
        }
    }
    if (!found) {
        printf("No messages found containing '%s'.\n", search_term);
    }
}

void delete_message() {
    //Implement: If the user chooses to delete a note/message, they will be prompted to enter the message/note id or title after which the application searches for and once found, deletes the note/message from the file.
    if (message_count == 0) {
        printf("\nNo messages to delete.\n");
        return;
    }
    view_messages();
    int delete_index;
    printf("\nEnter the number of the message to delete (0 to cancel): ");
    scanf("%d", &delete_index);
    // Consume the newline character left by scanf
    while (getchar() != '\n');

    if (delete_index > 0 && delete_index <= message_count) {
        // Adjust index to be 0-based
        delete_index--;
        // Shift remaining messages to fill the gap
        for (int i = delete_index; i < message_count - 1; i++) {
            strcpy(messages[i].text, messages[i + 1].text);
        }
        message_count--;
        printf("\nMessage %d deleted successfully.\n", delete_index + 1);
    } else if (delete_index != 0) {
        printf("\nInvalid message number.\n");
    } else {
        printf("\nDeletion cancelled.\n");
    }
}

void store_message() {
    //Implement a menu, to store message unchanged, to censor a message before storing, encrypt a messgae before storing or to decrypt a stored message.
    //To store unchanged, application should accept a title, and content.
    //To censor, the application should ask the user to enter a comma separated list of words that should be censored from the message/note. The program will implement an algorithm that redacts or censors the words provided from the text in the file. It should then accept a title for the message/note and the contents of the message/note, redact the words if they appear in the message/note and store the result appropriately in the defined file.
    //To enncrypt: If the user chooses to encrypt the message/note, the application should ask the user to enter a key to be used for the encryption. The program will implement a columnar transposition algorithm that encrypts the words provided from the message/note entered. It should accept the contents of the message/file, encrypt it following the guide below and store the result appropriately in the defined file.
    // To decrypt a stored message: If the user chooses to decrypt a stored message/note, the application should ask the user to enter a key to be used for the decryption. The program will implement an algorithm that decrypts the previous columnar transposition. It should then read the contents of the message and decrypt it following the guide below and display the results back to the user
    
    
    if (message_count >= MAX_MESSAGES) {
        printf("\nMessage storage is full.\n");
        return;
    }
    printf("\nEnter the new message: ");
    fgets(messages[message_count].text, MAX_MESSAGE_LENGTH, stdin);
    messages[message_count].text[strcspn(messages[message_count].text, "\n")] = 0; // Remove trailing newline
    message_count++;
    printf("\nMessage stored successfully.\n");
}

void search_word_phrase () {
    //Put code implementation here. If the user chooses to search for and view a note/message, they can be prompted to enter a word/phrase from the message/note id or title after which the application searches for messages matching this criteria and once found, displays the message id, title and the full message to screen. If the message/note was encrypted, the user should have the option to decrypt the message/note to view it as plain text.
}

void modify_message () {
    //Put code implementation here. If the user chooses to modify an existing note/message, they will be prompted to enter the message/note id or title after which the application searches for and once found, allows the user to modify the text stored. If the message/note was encrypted, the message/note should be decrypted before it is modified.
}


