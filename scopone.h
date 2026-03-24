#ifndef SCOPONE_H
#define SCOPONE_H

#include <stdio.h>

#define PLAYERS 4
#define DECK_SIZE 40
#define CARDS_PER_PLAYER (DECK_SIZE / PLAYERS)
#define TEAMS 2

/* MACRO FOR DEBUGGING PURPOSES */
//#define DEBUG // uncomment to show debugging information
#ifdef DEBUG
    #define debug(...) printf(__VA_ARGS__)
#else
    #define debug(...)
#endif

/* Translate a card's rank into its numeric value. */
char get_value(char rank);

/* Translate card's rank into numeric value for primiera */
char get_primiera_value(char rank);

/* Returns the maximum number of cards that sum to 'value' and, if 'indices' is not NULL, fills it with the selected card indices.
 * If there is a single card that matches the value, returns that card.
 * - table_ranks: the ranks of the cards on the table
 * - cards_on_table: the number of cards on the table
 * - value: the target value
 * - indices: the indices of the selected cards (if not NULL)
 */
int max_coverage(char table_ranks[], int cards_on_table, int value, int indices[]);

/* Reads the cards from standard input and distributes them to the players.
 * Assume that the input is valid.
 */
void deal_cards();

/* Return the index of the card that should be played this turn (within the cards held by the current player).
 * - table_suits: the suits of the cards on the table
 * - table_ranks: the ranks of the cards on the table
 * - cards_on_table: the number of cards on the table
 * - player: the player for this turn
 */
int select_card(char table_suits[], char table_ranks[], int cards_on_table, int player);

#endif //SCOPONE_H
