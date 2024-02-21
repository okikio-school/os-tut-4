/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2024, Okiki Ojo, Justin Fisher, Inder Singh
 * All rights reserved.
 */
#ifndef JEOPARDY_H_
#define JEOPARDY_H_

#define MAX_LEN 256

// Tokenizes user input into commands and parameters, facilitating command processing.
extern void tokenize(char *input, char *tokens[], int maxTokens);

// Displays the final game results, showing player rankings and scores.
extern void show_results(player *players, int num_players);

#endif /* JEOPARDY_H_ */