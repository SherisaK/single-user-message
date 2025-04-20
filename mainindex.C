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
                        printf("\nYou chose to view all messages.\n");
                        view_messages();
                        break;
                    case 2:
                        printf("\nYou can now search for a message.\n");
                        search_message();
                        break;
                    case 3:
                        printf("\nYou can now delete a message.\n");
                        delete_message();
                        break;
                    case 4:
                        printf("\nYou can now store a new message.\n");
                        store_message();
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
