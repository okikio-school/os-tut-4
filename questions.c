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
#include "questions.h"

question questions[NUM_QUESTIONS];

void stringToLower(char *s) {
    for (int i = 0; s[i]; i++) {
        s[i] = tolower((unsigned char)s[i]);
    }
}

// Initializes the array of questions for the game
void initialize_game(void)
{
    // initialize each question struct and assign it to the questions array
    int question_index = 0;

    for (int i = 0; i < NUM_CATEGORIES; i++) {
        printf("Category: %s", categories[i]);
        for (int j = 0; j < NUM_QUESTIONS_PER_CATEGORY; j++) {
            strcpy(questions[question_index].category, categories[i]);
            sprintf(questions[question_index].question, "Question %d in category %s", j+1, categories[i]);
            sprintf(questions[question_index].answer, "Answer %d in category %s", j+1, categories[i]);
            questions[question_index].value = (j + 1) * 100;
            questions[question_index].answered = false;


            printf("[Before] Question: %s, Answer: %s, Value: %i\n", questions[question_index].question, questions[question_index].answer, questions[question_index].value);
            stringToLower(questions[question_index].answer);
            printf("[After] Question: %s, Answer: %s, Value: %i\n", questions[question_index].question, questions[question_index].answer, questions[question_index].value);
        
            printf(" $%i (%i)", questions[question_index].value, j + 1);

            if (j < NUM_QUESTIONS_PER_CATEGORY - 1) {
                printf(",");
            }

            question_index++;
        }
        printf("\n");
    }
}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
    int question_index = 0;

    // print categories and dollar values for each unanswered question in questions array
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        printf("%s", categories[i]);
        for (int j = 0; j < NUM_QUESTIONS_PER_CATEGORY; j++) {
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

// Displays the question for the category and dollar value
void display_question(char *category, int value)
{
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            printf("Category: %s $%d\n", questions[i].category, questions[i].value);
            printf("Question: %s\n", questions[i].question);
            break;
        }
    }
}

// Returns true if the answer is correct for the question for that category and dollar value
ValidationResult valid_answer(char *category, int value, char *answer)
{
    // Declaration with initialization
    ValidationResult result = {};
    result.valid = false;
    result.question = 0;

    stringToLower(answer);

    // Look into string comparison functions
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        printf("%i. Answer -- Category: %s, Answer: %s, Value: %i\n", i, questions[i].category, questions[i].answer, questions[i].value);
        printf("%i. Input -- Category: %s, Answer: %s, Value: %i\n", i, category, answer, value);
        
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            result.question = i;

            if (strcmp(questions[i].answer, answer) == 0) {
                result.valid = true;
                return result;
            }

            break;
        }
        
        result.question = i;
    }

    printf("Result Question: %i, Valid: %s\n", result.question, result.valid ? "true" : "false");
    return result;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
    // lookup the question and see if it's already been marked as answered
     for (int i = 0; i < NUM_QUESTIONS; i++) {
        printf("Input -- Category: %s, Value: %i\n", category, value);
        printf("Answered -- Category: %s, Value: %i\n", questions[i].category, questions[i].value);

        if (
            strcmp(questions[i].category, category) == 0 && 
            questions[i].value == value
        ) {
            return questions[i].answered;
        }
    }
    return false;
}
