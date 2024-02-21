/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2024, Okiki Ojo, Justin Fisher, Inder Singh
 * All rights reserved.
 *
 * Part of the Jeopardy game simulation for Tutorial 4 of the Operating Systems course.
 * This file manages questions, including their initialization, display,
 * and the validation of player answers.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "questions.h" // Include the definitions for question structures and related functions.

// Global array storing all game questions.
question questions[NUM_QUESTIONS];

// Converts a string to lowercase to standardize answer checking.
void stringToLower(char *s) {
    for (int i = 0; s[i]; i++) {
        s[i] = tolower((unsigned char)s[i]);
    }
}

/**
 * Initializes the game questions, assigning each question to its category,
 * setting its dollar value, and marking it as unanswered. Also prints each
 * category and its questions to the console as they are initialized.
 */
void initialize_game(void) {
    int question_index = 0; // Track the index for the next question to initialize.

    // Loop through each category.
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        printf("Category: %s", categories[i]);

        // Initialize questions for the current category.
        for (int j = 0; j < NUM_QUESTIONS_PER_CATEGORY; j++) {
            // Copy category name, formulate the question and answer text,
            // set the dollar value, and mark as unanswered.
            strcpy(questions[question_index].category, categories[i]);
            sprintf(questions[question_index].question, "Question %d in category %s", j+1, categories[i]);
            sprintf(questions[question_index].answer, "Answer %d in category %s", j+1, categories[i]);
            
            questions[question_index].value = (j + 1) * 100;
            questions[question_index].answered = false;

            // Convert the answer to lowercase for case-insensitive comparison.
            stringToLower(questions[question_index].answer);        
            printf(" $%i (%i)", questions[question_index].value, j + 1);

            if (j < NUM_QUESTIONS_PER_CATEGORY - 1) {
                printf(",");
            }

            question_index++;
        }

        printf("\n");
    }
}

/**
 * Displays each category and the dollar values of unanswered questions.
 * Answered questions are shown with a strikethrough effect.
 */
void display_categories(void) {
    int question_index = 0; // Used to iterate through the questions array.

    // Loop through each category to display its unanswered questions.
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        printf("%s", categories[i]);
        for (int j = 0; j < NUM_QUESTIONS_PER_CATEGORY; j++) {
            // Display the question value, using strikethrough for answered questions.
            if (questions[question_index].answered) {
                printf(" \e[9m$%i\e[0m", questions[question_index].value);
            } else {
                printf(" $%i", questions[question_index].value);
            }

            if (j < NUM_QUESTIONS_PER_CATEGORY - 1) {
                printf(",");
            }

            question_index++;
        }

        printf("\n");
    }
}

/**
 * Displays the specified question for a given category and dollar value.
 * If no matching question is found, no output is produced.
 */
void display_question(char *category, int value) {
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        // Find and display the question matching the category and value.
        if (
            strcmp(questions[i].category, category) == 0 && 
            questions[i].value == value
        ) {
            printf("Category: %s $%d\n", questions[i].category, questions[i].value);
            printf("Question: %s\n", questions[i].question);
            break;
        }
    }
}

/**
 * Checks if a question in a given category and dollar value has already been answered.
 * 
 * @return true if the question has been answered, false otherwise.
 */
bool already_answered(char *category, int value) {
    // Search for the question and return its answered status.
     for (int i = 0; i < NUM_QUESTIONS; i++) {        
        if (
            strcmp(questions[i].category, category) == 0 && 
            questions[i].value == value
        ) {
            return questions[i].answered;
        }
    }

    return false; // No matching question found, or it hasn't been answered yet.
}

/**
 * Validates a player's answer for a specified category and dollar value.
 * Converts the provided answer to lowercase before comparison to ensure
 * case-insensitive checking.
 * 
 * @return A ValidationResult struct indicating if the answer was correct and
 *         the index of the question.
 */
ValidationResult valid_answer(char *category, int value, char *answer) {
    ValidationResult result = {false, 0}; // Initialize result as not valid.

    // Convert the answer to lowercase for comparison.
    stringToLower(answer);

    // Search for the question and compare the answer.
    for (int i = 0; i < NUM_QUESTIONS; i++) {        
        if (
            strcmp(questions[i].category, category) == 0 && 
            questions[i].value == value
        ) {
            result.question = i; // Record the question index.

            if (strcmp(questions[i].answer, answer) == 0) {
                result.valid = true; // Answer matches, mark as valid.
                return result;
            }

            break; // Question found and answer checked, no need to continue.
        }
        
        result.question = i;
    }

    return result; // Return the result (validity and question index).
}
