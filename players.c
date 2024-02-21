/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2024, Okiki Ojo, Justin Fisher, Inder Singh
 * All rights reserved.
 * 
 * Implements functions for managing players in a Jeopardy-style game.
 * This includes checking for player existence, updating scores, and displaying player information.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "players.h" // Include the definition of the player struct and prototypes of player-related functions.

/**
 * Checks if a given player already exists in the array of players.
 * 
 * @param players Array of player structs.
 * @param num_players The number of players in the array.
 * @param name The name of the player to check for.
 * @return true if the player exists, false otherwise.
 */
bool player_exists(player *players, int num_players, char *name) {
    for (int i = 0; i < num_players; i++) {
        // Use strcmp to compare the current player's name with the given name.
        if (strcmp(players[i].name, name) == 0) 
            // If a match is found, return true immediately.
            return true;
    }

    // If no match is found after checking all players, return false.
    return false;
}

/**
 * Updates the score of a specified player by adding the given score to their current score.
 * If the player does not exist, no action is taken.
 * 
 * @param players Array of player structs.
 * @param num_players The number of players in the array.
 * @param name The name of the player whose score is to be updated.
 * @param score The amount to add to the player's score.
 */
void update_score(player *players, int num_players, char *name, int score) {
    for (int i = 0; i < num_players; i++) {
        // Find the player by name.
        if (strcmp(players[i].name, name) == 0) {
            // Update the player's score by adding the given score.
            players[i].score += score;

            // Exit the loop early since the intended player has been found and updated.
            break;
        }
    }
}

/**
 * Prints the names and scores of all players.
 * 
 * @param players Array of player structs.
 * @param num_players The number of players in the array.
 */
void print_players(player *players, int num_players) {
    // Iterate over the array of players.
    for (int i = 0; i < num_players; i++) {
        // For each player, print their name and current score.
        printf("Player: %s, Score: $%d\n", players[i].name, players[i].score);
    }
}
