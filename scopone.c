#include "scopone.h"

/* Cards owned by the players
 * E.g. The suit and rank of the third card (card #2) of the second player (player #1) are stored in:
 *      suit[1][2] and rank[1][2]
 * NOTE: to indicate that a card has been played (and is unavailable) assign value 0 to the corresponding suit.
 */
char suit[PLAYERS][CARDS_PER_PLAYER];
char rank[PLAYERS][CARDS_PER_PLAYER];
int indexs[DECK_SIZE];
/**********************************************************/
/*         HELPER FUNCTIONS                               */
/**********************************************************/

/* Return max of a and b */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/* Translate a card's rank into its numeric value. */
char get_value(char rank) {
    if (rank == 'J')
        return 8;
    if (rank == 'Q')
        return 9;
    if (rank == 'K')
        return 10;
    if (rank == 'A')
        return 1;
    return (rank - '0');
}

/* Translate a card's rank into its numeric value for primiera calculation. */
char get_primiera_value(char rank) {
    if (rank == 'J' || rank == 'Q' || rank == 'K')
        return 10;
    if (rank == '7')
        return 21;
    if (rank == '6')
        return 18;
    if (rank == 'A')
        return 16;
    return (rank - '0' + 10);
}

/**********************************************************/
/*         CORE FUNCTIONS ALREADY PROVIDED                */
/**********************************************************/

/* Returns the maximum number of cards that sum to 'value' and, if 'indices' is not NULL, fills it with the selected card indices.
 * If there is a single card that matches the value, returns that card.
 * - table_ranks: the ranks of the cards on the table
 * - cards_on_table: the number of cards on the table
 * - value: the target value
 * - indices: the indices of the selected cards (if not NULL)
 */
int max_coverage(char table_ranks[], int cards_on_table, int value, int indices[]) {
    ////////////////////////////empty indicies
    if (indices != NULL) {
        for (int i = 0; i < DECK_SIZE; ++i) {
            indices[i] = -1;
        }
    }
    ////////////////////////////empty indicies

    // Coverage table (used only if a single matching card is not available on the table)
    int max_cards[cards_on_table + 1][value + 1];

    // Check if there is a card that matches the value
    for (int i = 0; i < cards_on_table; i++) {
        if (get_value(table_ranks[i]) == value) {
            if (indices != NULL) indices[0] = i;
            return 1;
        }
    }

    // Initialize coverage table
    for (int i = 0; i <= cards_on_table; i++)
        for (int sum = 0; sum <= value; sum++)
            max_cards[i][sum] = -1;

    max_cards[0][0] = 0;  // Base case: sum 0 with 0 elements

    // Fill coverage table
    for (int i = 1; i <= cards_on_table; i++) {
        for (int sum = 0; sum <= value; sum++) {

            // Case 1: do not take current element
            max_cards[i][sum] = max_cards[i - 1][sum];

            // Case 2: take current element if possible
            if (sum >= get_value(table_ranks[i - 1]) && max_cards[i - 1][sum - get_value(table_ranks[i - 1])] != -1) {
                max_cards[i][sum] = max(max_cards[i][sum],max_cards[i - 1][sum - get_value(table_ranks[i - 1])] + 1);
            }
        }
    }

    // If no combination found, return 0
    if (max_cards[cards_on_table][value] == -1) {
        return 0;
    }

    // Backtrack to find indices
    int sum = value;
    int i = cards_on_table;
    int count = 0;
    int idx = max_cards[cards_on_table][value];

    while (i > 0 && sum > 0) {
        if (sum >= get_value(table_ranks[i - 1]) && max_cards[i][sum] == max_cards[i - 1][sum - get_value(table_ranks[i - 1])] + 1) {
            if (indices != NULL) indices[--idx] = i - 1;  // store index if required
            count++;
            sum -= get_value(table_ranks[i - 1]);
        }
        i--;
    }

    // Return number of cards used
    return count;
}


/**********************************************************/
/*         CORE FUNCTIONS TO PROVIDE                    */
/**********************************************************/
int prMinSearch(int player, char array[]) {
    //local Variables
    char min = 100;
    char lastSuit = '\0';
    int index = 0;
    //for each array element up to number of player cards
    for (int i = 0; i < CARDS_PER_PLAYER; ++i) {
        if (array[i] < 10) continue;  // Excludes already played cards from calculation
        if (array[i] <= min ) {

            // handles case that 2 cards hold min value
            // picks first card that is not a diamond
            if (array[i] == min) {
                if (lastSuit != 'D'){
                    continue;
                }
                else if(suit[player][i] == 'D') continue;
            }
            // normal case
            min = array[i];
            lastSuit = suit[player][i];
            index = i;
        }
    }
    return index;
}   //retuns index of minimum primera value card Takes primera values array



/* Reads the cards from standard input and distributes them to the players.
 * Assume that the input is valid.
 */
void deal_cards() {
	int p = 0;
    int k = 0;
    char temp = ' ';
    while (1) {
        scanf(" %c",&temp);
        if (temp == 'E') break;
        suit[p][k] = temp;
        scanf(" %c",&temp);
        if (temp == 'E') break;
        rank[p][k] = temp;
        p++;
        if (p > 3) {
            p = 0;
            k++;
        }
    }
}


/* Return the index of the card that should be played this turn (within the cards held by the current player).
 * - table_suits: the suits of the cards on the table
 * - table_ranks: the ranks of the cards on the table
 * - cards_on_table: the number of cards on the table
 * - player: the player for this turn
 */
int select_card(char table_suits[], char table_ranks[], int cards_on_table, int player) {
    char rankVal[PLAYERS][CARDS_PER_PLAYER] = {0};     //temp var used to hold decoded value of ranks
    char primVal[CARDS_PER_PLAYER];                     //temp var to hold primera values of players hand
    int  cardPlay = -1;                                      //the index of the card the player must play
    for (int i = 0; i < 10; ++i) {
        rankVal[player][i] = get_value(rank[player][i]);
        primVal[i] = get_primiera_value(rank[player][i]);
    }   //for loop loads primVals and rankVals from current players rank

    /* If there are no cards on the table, play the card with the minimum value (in terms of primiera).
      * If there are multiple cards with this value, select the first one found that is not a diamond.
      */
    // count cards in hand
    int cardsinhand = 0;
    for (int i = 0; i < CARDS_PER_PLAYER; i++) {
        if (suit[player][i] != 0) cardsinhand++;
    }




    ///////////////////////////////////////////////////////////////////////////////////
    if (cards_on_table == 0) {
        return prMinSearch(player,primVal);
    }

    /////////////////////////////////////////////////////////////////////////////////

    /* [FILL HERE] */


    /* If the player has only one card in hand, play it. */
    ///////////////////////////////////////////////////////////////////////////////////
    if (cardsinhand == 1) {
        for (int i = 0; i < CARDS_PER_PLAYER; i++) {
            if (suit[player][i] != 0) {
                return i;

            }
        }
    }
    ///////////////////////////////////////////////////////////////////////////////////


    /* If the player has the settebello in hand and can make a capture, play it. */
    /////////////////////////////////////////////////////
    if (max_coverage(table_ranks,cards_on_table,7,indexs) > 0) {
        for (int i = 0; i < CARDS_PER_PLAYER; ++i) {
            if ( (suit[player][i] == 'D') && (rank[player][i]) == '7') {
                return i;
            }
        }
    }
    //////////////////////////////////////////////////////////

    /* If possible, make a scopa.
     * If multiple cards in hand can make a scopa and one is a diamond, choose that one.
     * Otherwise, play the first card found that allows a scopa.
     */
    int maxScope[CARDS_PER_PLAYER];
    for (int i = 0; i < CARDS_PER_PLAYER; ++i) {
        maxScope[i] = -1;
    }
    int numScope = 0;
    for (int i = 0; i < CARDS_PER_PLAYER; ++i) {
       if (rank[player][i] == 0) continue;
        if ( max_coverage(table_ranks,cards_on_table, get_value(rank[player][i]),indexs) == cards_on_table) {
            maxScope[i] = i;
            numScope++;
            if (suit[player][i] == 'D') {
                return i;
            }
        }

    }
    for (int i = 0; i < CARDS_PER_PLAYER; ++i) {
        if (maxScope[i] != -1) return maxScope[i];
    }
    /* [FILL HERE] */


    /* If the player has a rank-7 card and there is a 7 on the table, play that card. */
    int sevenTable = 0;
    int sevenHand = 0;
    for (int i = 0; i < CARDS_PER_PLAYER; ++i) {
        if (table_ranks[i] == '7') sevenTable = 1;

    }
    for (int i = 0; i < CARDS_PER_PLAYER; ++i) {
        if (rank[player][i] == '7') sevenHand = 1;
        if (sevenHand && sevenTable) return i;
    }

    /* Play the highest-ranked diamond that allows a capture, if one is available. */
    for (int i = CARDS_PER_PLAYER-1; i >= 0; --i) {
        if (suit[player][i] == 'D') {
            if (max_coverage(table_ranks, cards_on_table, rankVal[player][i],indexs) > 0) return i;
        }
    }
    /* [FILL HERE] */


    /* Play the card that allows the maximum-coverage capture (i.e., captures the largest number of cards).
     * If multiple cards qualify, play the first one found.
     */
    int maxCap = 0;
    int temp = 0;
    int tempi;
    for (int i = 0; i < CARDS_PER_PLAYER; ++i ) {
        if (rank[player][i] == 0) continue;
        if ( (temp = max_coverage(table_ranks, cards_on_table,rankVal[player][i],indexs)) > maxCap) {
            maxCap = temp;
            tempi = i;
        }
    }
    if (maxCap != 0) return tempi;
    /* [FILL HERE] */
    int maxRank = 0;
    int Ihold = 0;
    for (int i = 0; i < CARDS_PER_PLAYER; ++i) {
        if (rankVal[player][i] > maxRank) {
            maxRank = rankVal[player][i];
            Ihold = i;
        }
    }
    return Ihold;
    /* A capture is not possible: play the card with the maximum rank.
       If there are multiple cards with this rank, select the first one found that is not a diamond. */

    /* [FILL HERE] */
    //error message
    if (cardPlay == -1) {
        printf("error selecting card!!\n");
        cardPlay = 0;
    }
    return cardPlay; // change the return value
}
