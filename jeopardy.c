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
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define NUM_PLAYERS 4

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
    char* back = s + strlen(s);
    while (isspace((unsigned char)*--back));
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
    char *token = strtok(trim(input), " "); // Find the first token

    while (token != NULL && tokenCount < maxTokens) {
        tokens[tokenCount++] = token; // Store the token
        token = strtok(NULL, " "); // Get the next token
    }

    tokens[tokenCount] = token; // Null-terminate the array of tokens
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

    // Display the game introduction and initialize the questions
    initialize_game();

    // Prompt for players names
    for (int i = 0; i < NUM_PLAYERS; i++) {
        printf("Enter name for player %d: ", i + 1);
        fgets(buffer, BUFFER_LEN, stdin);
        strtok(buffer, "\n"); // Remove trailing newline
        strcpy(players[i].name, buffer);
        players[i].score = 0; // initialize each of the players in the array
    }

    // Perform an infinite loop getting command input from users until game ends
    while (fgets(buffer, BUFFER_LEN, stdin) != NULL) {
        // Tokenize input
        tokenize(buffer, tokens, BUFFER_LEN);

        // Execute the game until all questions are answered
        if (strcmp(tokens[0], "exit") == 0) {
            // If the user types "exit", end the game
            break;
        } else if (strcmp(tokens[0], "display") == 0) {
            // If the user types "display", display categories and questions
            display_categories();
        } else if (strcmp(tokens[0], "pick") == 0 && tokens[1] != NULL && tokens[2] != NULL) {
            // If the user picks a question
            char *category = tokens[1];
            int value = atoi(tokens[2]);
            int i;
            if (!already_answered(category, value)) {
                // Display the question
                display_question(category, value);
                printf("Enter your answer: ");
                fgets(buffer, BUFFER_LEN, stdin);
                strtok(buffer, "\n"); // Remove trailing newline
                
                // Validate the answer
                if (valid_answer(category, value, buffer)) {
                    printf("Correct answer! You earned %d points.\n", value);
                    // Update player's score
                    for (i = 0; i < NUM_PLAYERS; i++) {
                        if (strcmp(players[i].name, tokens[3]) == 0) {
                            players[i].score += value;
                            break;
                        }
                    }
                } else {
                    printf("Incorrect answer! The correct answer is: %s\n", questions[i].answer);
                }
            } else {
                printf("Question already answered.\n");
            }
        } else {
            printf("Invalid command.\n");
        }
    }

    // Display the final results and exit
    show_results(players, NUM_PLAYERS);
    return EXIT_SUCCESS;
}
