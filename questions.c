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

// Initializes the array of questions for the game
void initialize_game(void)
{
    // initialize each question struct and assign it to the questions array
    int question_index = 0;

    for (int i = 0; i < NUM_CATEGORIES; i++) {
        for (int j = 0; j < NUM_QUESTIONS_PER_CATEGORY; j++) {
            strcpy(questions[question_index].category, categories[i]);
            sprintf(questions[question_index].question, "Question %d in category %s", j+1, categories[i]);
            sprintf(questions[question_index].answer, "Answer %d in category %s", j+1, categories[i]);
            questions[question_index].value = (j + 1) * 100;
            questions[question_index].answered = false;
            question_index++;
        }
    }
}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
    // print categories and dollar values for each unanswered question in questions array
    for (int i = 0; i < NUM_CATEGORIES; i++) {
        printf("%s\n", categories[i]);
    }
}

// Displays the question for the category and dollar value
void display_question(char *category, int value)
{
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            printf("Category: %s\n", questions[i].category);
            printf("Question: %s\n", questions[i].question);
            printf("Value: %d\n", questions[i].value);
            break;
        }
    }
}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
    // Look into string comparison functions
      for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            if (strcmp(questions[i].answer, answer) == 0) {
                return true;
            }
            break;
        }
    }
    return false;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
    // lookup the question and see if it's already been marked as answered
     for (int i = 0; i < NUM_QUESTIONS; i++) {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value) {
            return questions[i].answered;
        }
    }
    return false;
}
