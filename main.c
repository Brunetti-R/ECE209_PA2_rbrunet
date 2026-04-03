/* ECE 209, Spring 2026
 * Simulator of Scopone Scientifico card game
 * (1) User enters expression: <turns> [<card>]x40 E, where <card> = <suit><value>
 * (2) Print outcome of the game.
*/

#include "scopone.h"
#include <string.h>
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
    int capture = 0;   // indicator value returned by capture function
    int player = 0;
    int playCard;
    int cards_on_table = 0;
    /* Print prompt for user input */
    printf("Enter the number of turns and the cards in the deck (terminated by 'E'):\n");

    /* Read the number of turns to be played. */
    scanf("%d ", &num_turns);
    char suitsWon[num_turns][PLAYERS][DECK_SIZE];
    char ranksWon[num_turns][PLAYERS][DECK_SIZE];
    for (int i = 0; i < num_turns; i++) {
        for (int j = 0; j < PLAYERS; j++) {
            for (int k = 0; k < DECK_SIZE; k++) {
                suitsWon[i][j][k] = '\0';
            }
        }
    }
    for (int i = 0; i < num_turns; i++) {
        for (int j = 0; j < PLAYERS; j++) {
            for (int k = 0; k < DECK_SIZE; k++) {
                ranksWon[i][j][k] = '\0';
            }
        }
    }
    /* Distribute the cards to the players (assume that the input is valid). */
    deal_cards();

    /* Print out the card distribution among players */
    print_card_distribution();

    /* Game play for the specified number of turns (num_turns). */
    for (int turn = 0; turn < num_turns; turn++) {

        /* Select the card to be played */
        playCard = select_card(table_suits, table_ranks, cards_on_table, player);
        if (capture == 0) {                                      //place card on table if no capture
            for (int i = 0; i < DECK_SIZE; ++i) {
               if (table_ranks[i] == 0) {
                   table_suits[i] = suit[player][playCard];
                   table_ranks[i] = rank[player][playCard];
                   break;
               } else continue;
            }
        }
        cards_on_table = 0;
        for (int i = 0; i < DECK_SIZE;++i) {

            if (table_suits[i] != 0) cards_on_table++;                          //counts cards on table
        }
    	/* [FILL HERE] */

        /* - Update the list of cards on the table and in hand.
         * - Keep track of the cards won by each team and the number of scopas (sweeps) made.
         *
         * - Print the outcome of each turn: card played, cards won, cards remaining on the table (see format in project specifications).
         */
    	printf("turn %d:: player: %d, card played: %c%c, cards won: [", turn, player, suit[player][playCard],
               rank[player][playCard]);
        for (int i = 0; i < strlen(suitsWon[turn][player]);++i ) {
            printf(" %c%c",suitsWon[turn][player][i],ranksWon[turn][player][i]);
        }
        printf(" ] table: [");
        for (int i = 0; i < cards_on_table; ++i) {
            printf(" %c%c",table_suits[i],table_ranks[i]);
        }
        printf(" ]\n");

        /* [FILL HERE] */
        ++player;
        if (player > 3) player = 0;
    } // end loop

    /* If all 40 turns have been played and there are cards left on the table, assign them
    * to the last team that made a capture.
    */

    /* [FILL HERE] */
if (num_turns != 0) {
    /* Compute the scores and print them, along with the winner (see format in project specifications). */

    for (int team = 0; team < 2; ++team) {
        printf("team %d:: cards won: [",team);
        for (player = 0; player < 4; ++player){
            if ( (team + player) % 2) continue;

            for (int t = 0; t < num_turns; ++t) {
                for (int i = 0; i < strlen(suitsWon[t][player]); ++i) {
                    printf(" %c%c",suitsWon[t][player][i],ranksWon[t][player][i]);
                }
            }


        }
        printf(" ] scores = [cards=0, diamonds=0, settebello=0, primiera=0, scopa=0]\n");

        /* [FILL HERE] */
    }
}/* The program always returns successfully. */
    return 0;
}
