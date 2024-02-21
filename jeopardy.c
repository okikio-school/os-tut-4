/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2024, Okiki Ojo, Justin Fisher, Inder Singh
 * All rights reserved.
 *
 * This file contains the main logic for a command-line version of Jeopardy.
 * It includes functions for trimming strings, comparing player scores, displaying game results,
 * and processing user input to control the game flow.
 *
 * Players can answer questions from selected categories to earn points. The game tracks
 * each player's score and displays the final results once all questions have been answered or
 * when the game is exited.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "questions.h" // Includes the definitions and functions related to questions
#include "players.h"   // Includes player struct and related functionalities
#include "jeopardy.h"  // May include game-wide constants, structs, and prototypes

#define BUFFER_LEN 256  // General purpose buffer length for input and strings
#define NUM_PLAYERS 4   // Fixed number of players for this game

#define WHAT_IS "what is"
#define WHO_IS "who is"

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

// Tokenizes user input into commands and parameters, facilitating command processing.
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

// Compares two players based on their scores to facilitate sorting.
int compare_players(const void *a, const void *b) {
    const player *playerA = (const player *)a;
    const player *playerB = (const player *)b;

    // Directly compare the scores
    if (playerA->score > playerB->score) return -1; // For descending order
    else if (playerA->score < playerB->score) return 1;
    else return 0;
}

// Displays the final game results, showing player rankings and scores.
void show_results(player *players, int num_players) {
    printf("All questions have been answered. The game is over.\n");
    printf("Final Results:\n");

    qsort(players, num_players, sizeof(player), compare_players);
    for (int i = 0; i < num_players; i++) {
        if (i == 0) {
            printf("%i. (Winner) ", i + 1);
        } else {
            printf("%i. ", i + 1);
        }
        
        printf("Player: %s, Score: $%i\n", players[i].name, players[i].score);
    }
}

int main() {
    // An array of 4 players, may need to be a pointer if you want it set dynamically
    player players[NUM_PLAYERS];
    
    // Input buffer and and commands
    char buffer[BUFFER_LEN]; // Buffer for reading input
    char *tokens[BUFFER_LEN]; // Array to hold tokenized input for command processing

    // Initial setup: prompt for player names and initialize their scores
    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("Enter name for player %d: ", i + 1);
        fgets(buffer, BUFFER_LEN, stdin);

        strtok(buffer, "\n"); // Remove trailing newline
        strcpy(players[i].name, buffer);
        players[i].score = 0; // initialize each of the players in the array
    }

    // Game setup: display intro and initialize questions
    initialize_game();
    print_players(players, NUM_PLAYERS);

    // Help command details
    char* help = "Available commands:\n* display\n* exit\n* pick [category] [value] [user]\n  e.g. pick databases 100 User1";
    printf("%s\n", help);

    // Main game loop: process user commands until all questions are answered or user exits
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL) {
        tokenize(buffer, tokens, BUFFER_LEN); // Tokenize the user input for command processing
        if (tokens[0] == NULL) continue; // Skip empty input

        // Command processing
        if (strcmp(tokens[0], "exit") == 0) {
            break; // Exit the game loop
        } else if (strcmp(tokens[0], "help") == 0) {
            printf("%s\n", help); // Display help information
        } else if (strcmp(tokens[0], "display") == 0) {
            display_categories(); // Show available categories and questions
            print_players(players, NUM_PLAYERS); // Show player scores
        } else if (strcmp(tokens[0], "pick") == 0 && tokens[1] != NULL && tokens[2] != NULL && tokens[3] != NULL) {
            // If the user picks a question
            char *category = tokens[1];
            char *user = tokens[3];

            // Find the player index by name
            int playerIndex = -1;
            for (int i = 0; i < NUM_PLAYERS; i++) {
                if (strcmp(players[i].name, user) == 0) {
                    playerIndex = i;
                    break;
                }
            }

            // Print an error message if the player is not found
            if (playerIndex == -1) {
                printf("Invalid player \"%s\". Please try again.\n", user);
                continue;
            }

            // Remove the dollar sign from the value (if any)
            tokens[2] = strtok(tokens[2], "$");
            int value = atoi(tokens[2]);

            // Check if the question has already been answered
            if (!already_answered(category, value)) {
                char answer[BUFFER_LEN] = { 0 };
                char* trimmedAnswer;

                // Display the question
                display_question(category, value);
                printf("Enter your answer: ");

                // Get the user's answer
                while (fgets(answer, BUFFER_LEN, stdin) != NULL) {
                    // Trim leading and trailing whitespace
                    stringToLower(answer);
                    trimmedAnswer = trim(answer);
                    if (trimmedAnswer[0] == '\0') continue;

                    // Check if the answer starts with "What is" or "Who is"
                    if (strncmp(trimmedAnswer, WHAT_IS, strlen(WHAT_IS)) == 0) {
                        trimmedAnswer = trimmedAnswer + strlen(WHAT_IS);
                        break;
                    } else if (strncmp(trimmedAnswer, WHO_IS, strlen(WHO_IS)) == 0) {
                        trimmedAnswer = trimmedAnswer + strlen(WHO_IS);
                        break;
                    } else {
                        // Prompt the user to re-enter the answer
                        printf("The correct form for your response would be 'What is...' or 'Who is...'? Please try again.\n");
                        printf("Enter your answer: ");
                    }
                }

                // Trim leading and trailing whitespace from the answer
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

                // Check if all questions have been answered
                bool allAnswered = true;
                for (int i = 0; i < NUM_QUESTIONS; i++) {
                    if (!questions[i].answered) {
                        allAnswered = false;
                    }
                }

                // If all questions have been answered, display the final results and exit
                if (allAnswered) { break; }
            } else {
                printf("Question already answered.\n");
            }
        } else {
            printf("Invalid command.\n%s\n", help); // Handle unrecognized commands
        }
    }

    // End of game: display final results
    show_results(players, NUM_PLAYERS);
    return EXIT_SUCCESS;
}
