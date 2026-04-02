/* ECE 209, Spring 2026
 * Simulator of Scopone Scientifico card game
 * (1) User enters expression: <turns> [<card>]x40 E, where <card> = <suit><value>
 * (2) Print outcome of the game.
*/

#include "scopone.h"

/* Cards held by the players (see scopone.c) */
extern char suit[PLAYERS][CARDS_PER_PLAYER];
extern char rank[PLAYERS][CARDS_PER_PLAYER];

/* Print the cards held by each player. */
void print_card_distribution() {
    for (int i = 0; i < PLAYERS; i++) {
        printf("player %d:: ", i);
        for (int j = 0; j < CARDS_PER_PLAYER; j++) {
            if (suit[i][j] == 0) printf("-- ");
            else printf("%c%c ", suit[i][j], rank[i][j]);
        }
        printf("\n");
    }
}

/* Main: execution and score computation */
int main(void) {
    /* [FILL HERE (required local variables)] */
    int num_turns = 0;
    char table_suits[DECK_SIZE] = {0};
    char table_ranks[DECK_SIZE] = {0};
    int cards_on_table = {0};
    int player = 0;
    int playCard;
    /* Print prompt for user input */
    printf("Enter the number of turns and the cards in the deck (terminated by 'E'):\n");

    /* Read the number of turns to be played. */
    scanf("%d ", &num_turns);

    /* Distribute the cards to the players (assume that the input is valid). */
    deal_cards();

    /* Print out the card distribution among players */
    print_card_distribution();

    /* Game play for the specified number of turns (num_turns). */
    for (int turn = 0; turn < num_turns; turn++) {

        /* Select the card to be played */
        playCard = select_card(table_suits, table_ranks, cards_on_table, player);
    	/* [FILL HERE] */

        /* - Update the list of cards on the table and in hand.
         * - Keep track of the cards won by each team and the number of scopas (sweeps) made.
         *
         * - Print the outcome of each turn: card played, cards won, cards remaining on the table (see format in project specifications).
         */
    	printf("turn %d:: player: %d, played: %c%c, cards won: [ ] table: [ ]",turn,player, suit[player][playCard], rank[player][playCard]);
	    /* [FILL HERE] */

    } // end loop

    /* If all 40 turns have been played and there are cards left on the table, assign them
    * to the last team that made a capture.
    */

    /* [FILL HERE] */

    /* Compute the scores and print them, along with the winner (see format in project specifications). */

    /* [FILL HERE] */

    /* The program always returns successfully. */
    return 0;
}
