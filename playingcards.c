#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DECK_SIZE 52
#define SUIT_COUNT 4
#define RANK_COUNT 13
#define PLAYER_COUNT 4
#define CARDS_PER_PLAYER 5

typedef struct
{
    int rank; /* 0 = Ace, 1..9 = 2..10, 10 = Jack, 11 = Queen, 12 = King */
    int suit; /* 0 = Hearts, 1 = Diamonds, 2 = Clubs, 3 = Spades */
} Card;

const char *ranks[RANK_COUNT] = {
    "A", "2", "3", "4", "5", "6", "7",
    "8", "9", "10", "J", "Q", "K"};

const char *suits[SUIT_COUNT] = {
    "Hearts", "Diamonds", "Clubs", "Spades"};

void createDeck(Card deck[])
{
    int index = 0;
    for (int suit = 0; suit < SUIT_COUNT; suit++)
    {
        for (int rank = 0; rank < RANK_COUNT; rank++)
        {
            deck[index].rank = rank;
            deck[index].suit = suit;
            index++;
        }
    }
}

/* Fisher-Yates shuffle.
   For each position i, choose j uniformly from 0..i and swap deck[i], deck[j]. */
void shuffleDeck(Card deck[])
{
    for (int i = DECK_SIZE - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void printCard(Card card)
{
    printf("%s of %s", ranks[card.rank], suits[card.suit]);
}

void displayDeck(const Card deck[], int showFullDeck)
{
    const int previewCount = 8;
    const int cardsToDisplay = showFullDeck ? DECK_SIZE : previewCount;
    const char *title = showFullDeck ? "SHUFFLED DECK" : "SHUFFLED DECK PREVIEW";
    const int leftPadding = (53 - (int)strlen(title)) / 2;
    const int rightPadding = 53 - (int)strlen(title) - leftPadding;

    printf("\n+--------------------------+--------------------------+\n");
    printf("|%*s%s%*s|\n", leftPadding, "", title, rightPadding, "");
    printf("+--------------------------+--------------------------+\n");

    for (int i = 0; i < cardsToDisplay; i += 2)
    {
        char left[25];
        char right[25] = "";

        snprintf(left, sizeof(left), "%2d. %s of %s",
                 i + 1, ranks[deck[i].rank], suits[deck[i].suit]);
        if (i + 1 < cardsToDisplay)
        {
            snprintf(right, sizeof(right), "%2d. %s of %s",
                     i + 2, ranks[deck[i + 1].rank], suits[deck[i + 1].suit]);
        }

        printf("| %-24s | %-24s |\n", left, right);
    }

    printf("+--------------------------+--------------------------+\n");
    if (!showFullDeck)
    {
        printf("Preview: 8 of %d cards. Use --full-deck to display all cards.\n", DECK_SIZE);
    }
}

int validateDeck(const Card deck[])
{
    int seen[SUIT_COUNT][RANK_COUNT] = {{0}};

    for (int i = 0; i < DECK_SIZE; i++)
    {
        int suit = deck[i].suit;
        int rank = deck[i].rank;

        if (suit < 0 || suit >= SUIT_COUNT ||
            rank < 0 || rank >= RANK_COUNT)
        {
            return 0;
        }

        if (seen[suit][rank] != 0)
        {
            return 0; /* Duplicate card found */
        }
        seen[suit][rank] = 1;
    }

    for (int suit = 0; suit < SUIT_COUNT; suit++)
    {
        for (int rank = 0; rank < RANK_COUNT; rank++)
        {
            if (seen[suit][rank] != 1)
            {
                return 0; /* A card is missing */
            }
        }
    }
    return 1;
}

void dealCards(const Card deck[], int playerCount, int cardsPerPlayer)
{
    int totalDealt = playerCount * cardsPerPlayer;

    printf("\n====================================================\n");
    printf("        DEALING %d CARDS TO EACH PLAYER\n", cardsPerPlayer);
    printf("====================================================\n");

    for (int player = 0; player < playerCount; player++)
    {
        printf("Player %d: ", player + 1);
        for (int k = 0; k < cardsPerPlayer; k++)
        {
            int index = player * cardsPerPlayer + k;
            printCard(deck[index]);
            if (k < cardsPerPlayer - 1)
            {
                printf("  |  ");
            }
        }
        printf("\n");
    }

    printf("\nCards dealt: %d\n", totalDealt);
    printf("Cards remaining in deck: %d\n",
           DECK_SIZE - totalDealt);
}

static int parsePositiveInt(const char *text, int minValue, int maxValue)
{
    char *end = NULL;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);

    if (errno != 0 || end == text || *end != '\0')
    {
        return -1;
    }

    if (value < minValue || value > maxValue)
    {
        return -1;
    }

    return (int)value;
}

int main(int argc, char *argv[])
{
    Card deck[DECK_SIZE];
    int playerCount = PLAYER_COUNT;
    int cardsPerPlayer = CARDS_PER_PLAYER;
    int showFullDeck = 0;
    unsigned int seed = (unsigned int)time(NULL);

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
        {
            printf("Usage: %s [players] [cards_per_player] [seed] [--full-deck]\n", argv[0]);
            printf("Defaults: %d players, %d cards each, random seed from system time.\n",
                   PLAYER_COUNT, CARDS_PER_PLAYER);
            return 0;
        }
        else if (strcmp(argv[i], "--full-deck") == 0 || strcmp(argv[i], "-f") == 0)
        {
            showFullDeck = 1;
        }
        else if (i == 1)
        {
            int parsedPlayers = parsePositiveInt(argv[i], 1, DECK_SIZE);
            if (parsedPlayers == -1)
            {
                fprintf(stderr, "Invalid player count. Use a value between 1 and %d.\n", DECK_SIZE);
                return 1;
            }
            playerCount = parsedPlayers;
        }
        else if (i == 2)
        {
            int parsedCards = parsePositiveInt(argv[i], 1, DECK_SIZE);
            if (parsedCards == -1)
            {
                fprintf(stderr, "Invalid cards-per-player value. Use a value between 1 and %d.\n", DECK_SIZE);
                return 1;
            }
            cardsPerPlayer = parsedCards;
        }
        else if (i == 3)
        {
            int parsedSeed = parsePositiveInt(argv[i], 1, 2147483647);
            if (parsedSeed == -1)
            {
                fprintf(stderr, "Invalid seed value. Use a value between 1 and 2147483647.\n");
                return 1;
            }
            seed = (unsigned int)parsedSeed;
        }
        else
        {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 1;
        }
    }

    if (playerCount * cardsPerPlayer > DECK_SIZE)
    {
        fprintf(stderr, "The chosen deal size exceeds the deck size. Use at most %d total cards.\n",
                DECK_SIZE);
        return 1;
    }

    srand(seed);
    createDeck(deck);

    printf("\n=============================================\n");
    printf("       RANDOMIZED ALGORITHMS: PLAYING CARDS\n");
    printf("=============================================\n");
    printf("Deck status : Standard 52-card deck\n");
    printf("Mode        : %d players | %d cards each | seed %u\n",
           playerCount, cardsPerPlayer, seed);

    shuffleDeck(deck);
    displayDeck(deck, showFullDeck);

    if (validateDeck(deck))
    {
        printf("Validation  : PASS  |  All %d cards appear exactly once.\n", DECK_SIZE);
    }
    else
    {
        printf("Validation  : FAIL  |  Deck contains an error.\n");
        return 1;
    }

    dealCards(deck, playerCount, cardsPerPlayer);
    return 0;
}
