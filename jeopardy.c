/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define NUM_PLAYERS 4

#define WHAT_IS "what is"
#define WHO_IS "who is"

// Put global environment variables here

/**
 * Trims leading whitespace characters from a string.
 * 
 * This function moves the pointer to the first non-whitespace character
 * in the string, effectively "trimming" the whitespace from the start.
 * 
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the first non-whitespace character in the string.
 */
char* ltrim(char *s) {
    while (isspace((unsigned char)*s)) {
        s++;
    }
    return s;
}

/**
 * Trims trailing whitespace characters from a string.
 * 
 * This function iterates from the end of the string moving backwards
 * and places a null terminator '\0' after the last non-whitespace
 * character, effectively "trimming" the whitespace from the end.
 * 
 * @param s Pointer to the string to be trimmed.
 * @return The same pointer passed in as the parameter, now pointing to a string
 *         with trailing whitespace removed.
 */
char* rtrim(char *s) {
    if (!*s) // Check if the string is empty
        return s;

    char* back = s + strlen(s) - 1;
    while (back >= s && isspace((unsigned char)*back))
        back--;
    *(back + 1) = '\0';
    return s;
}

/**
 * Trims both leading and trailing whitespace characters from a string.
 * 
 * This function first trims the leading whitespace by calling `ltrim`
 * and then trims the trailing whitespace by calling `rtrim`.
 * 
 * @param s Pointer to the string to be trimmed.
 * @return A pointer to the trimmed string. If the string is all whitespace,
 *         the returned pointer will point to the terminating null character.
 */
char* trim(char *s) {
    return rtrim(ltrim(s));  
}

// Processes the answer from the user containing what is or who is and tokenizes it to retrieve the answer.
void tokenize(char *input, char *tokens[], int maxTokens) {
    int tokenCount = 0;
    // Trim leading and trailing whitespace including newlines
    char *trimmedInput = trim(input);
    char *token = strtok(trimmedInput, " "); // Find the first token
    while (token != NULL && tokenCount < maxTokens) {
        tokens[tokenCount++] = token; // Store the token
        token = strtok(NULL, " "); // Get the next token
    }

    tokens[tokenCount] = NULL; // Null-terminate the array of tokens
}

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(player *players, int num_players) {
    // Display results
    printf("Final Results:\n");
    for (int i = 0; i < num_players; i++) {
        printf("%s: %d\n", players[i].name, players[i].score);
    }
}

int main() {
    // An array of 4 players, may need to be a pointer if you want it set dynamically
    player players[NUM_PLAYERS];
    
    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };
    char *tokens[BUFFER_LEN]; // Declare the tokens array here

    // Prompt for players names
    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("Enter name for player %d: ", i + 1);
        fgets(buffer, BUFFER_LEN, stdin);
        strtok(buffer, "\n"); // Remove trailing newline
        strcpy(players[i].name, buffer);
        players[i].score = 0; // initialize each of the players in the array
    }

    // Display the game introduction and initialize the questions
    initialize_game();
    print_players(players, NUM_PLAYERS);

    char* help = "Available commands:\n* display\n* exit\n* pick [category] [value] [user]\n  e.g. pick databases 100 User1";
    printf("%s\n", help);

    // Perform an infinite loop getting command input from users until game ends
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL) {
        // Tokenize input
        tokenize(buffer, tokens, BUFFER_LEN);
        if (tokens[0] == NULL) {
            continue;
        }

        // Execute the game until all questions are answered
        if (strcmp(tokens[0], "exit") == 0) {
            // If the user types "exit", end the game
            break;
        } else if (strcmp(tokens[0], "help") == 0) {
            // If the user types "help", display supported commands for the game
            printf("%s\n", help);
        } else if (strcmp(tokens[0], "display") == 0) {
            // If the user types "display", display categories and questions
            display_categories();
            print_players(players, NUM_PLAYERS);
        } else if (strcmp(tokens[0], "pick") == 0 && tokens[1] != NULL && tokens[2] != NULL && tokens[3] != NULL) {
            // If the user picks a question
            char *category = tokens[1];
            char *user = tokens[3];

            int playerIndex = -1;
            for (int i = 0; i < NUM_PLAYERS; i++) {
                if (strcmp(players[i].name, user) == 0) {
                    playerIndex = i;
                    break;
                }
            }

            if (playerIndex == -1) {
                printf("Invalid player \"%s\". Please try again.\n", user);
                continue;
            }

            // Remove the dollar sign from the value (if any)
            tokens[2] = strtok(tokens[2], "$");
            int value = atoi(tokens[2]);
            if (!already_answered(category, value)) {
                char answer[BUFFER_LEN] = { 0 };
                char* trimmedAnswer;

                // Display the question
                display_question(category, value);
                printf("Enter your answer: ");
                while (fgets(answer, BUFFER_LEN, stdin) != NULL) {
                    // Trim leading and trailing whitespace
                    stringToLower(answer);
                    trimmedAnswer = trim(answer);
                    if (trimmedAnswer[0] == NULL) continue;

                    if (strncmp(trimmedAnswer, WHAT_IS, strlen(WHAT_IS)) == 0) {
                        trimmedAnswer = trimmedAnswer + strlen(WHAT_IS);
                        break;
                    } else if (strncmp(trimmedAnswer, WHO_IS, strlen(WHO_IS)) == 0) {
                        trimmedAnswer = trimmedAnswer + strlen(WHO_IS);
                        break;
                    } else {
                        printf("The correct form for your response would be 'What is...' or 'Who is...'? Please try again.\n");
                    }
                }

                trimmedAnswer = trim(trimmedAnswer);
                
                // Validate the answer
                ValidationResult ans = valid_answer(category, value, trimmedAnswer);
                if (ans.valid) {
                    printf("Correct answer! User %s earned %d points.\n", user, value);
                    
                    // Update player's score
                    players[playerIndex].score += value;
                } else {
                    printf("Incorrect answer! The correct answer is: %s\n", questions[ans.question].answer);
                }

                // Mark the question as answered
                questions[ans.question].answered = true;
            } else {
                printf("Question already answered.\n");
            }
        } else {
            printf("Invalid command.\n%s\n", help);
        }
    }

    // Display the final results and exit
    show_results(players, NUM_PLAYERS);
    return EXIT_SUCCESS;
}
