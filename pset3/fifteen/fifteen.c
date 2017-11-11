/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();
        
        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }
    
    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).  
 */
void init(void)
{
    // board[d][d]
    int current_number = d * d - 1;
    // loop through the board and add number to each position
    for (int i = 0; i < d; i++)
    {
        for (int k = 0; k < d; k++)
        {
            board[i][k] = current_number;
            current_number -= 1;
        }
    }
    // reverse the position if the dimension is even
    if (d % 2 == 0)
    {
        int temp = board[d - 1][d - 2];
        board[d - 1][d - 2] = board[d - 1][d - 3];
        board[d - 1][d - 3] = temp;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // loop through the 2D array and print it
    for (int i = 0; i < d; i++)
    {
        for (int k = 0; k < d; k++)
        {
            
            if (board[i][k] == 0)
                printf(" _ ");
            else if (board[i][k] < 10)
                printf(" %i ", board[i][k]);
            else
                printf("%i ", board[i][k]);
        }
        printf("\n");
    }
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // store current position and target position
    int y_position = -1;
    int x_position = -1;
    int current_x_position = -1;
    int current_y_position = -1;
    
    for (int i = 0; i < d; i++)
    {
        for (int k = 0; k < d; k++)
        {
            if (board[i][k] == tile)
            {
                x_position = i;
                y_position = k;
            }
            if (board[i][k] == 0)
            {
                current_x_position = i;
                current_y_position = k;
            }
        }
    }
    
    // check if the number exists
    if (x_position == -1 || y_position == -1 || tile == 0)
        return false;
    // check if the position is movable
    if (abs(current_x_position - x_position) >= 1 && abs(current_y_position - y_position) >= 1)
        return false;
    // check if the position is movable
    if (abs(current_x_position - x_position) >= 2 || abs(current_y_position - y_position) >= 2)
        return false;
    // move the position by swapping values
    int temp = board[current_x_position][current_y_position];
    board[current_x_position][current_y_position] = board[x_position][y_position];
    board[x_position][y_position] = temp;
    
    
    return true;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{

    // current to check
    int current_number =  1;

    // loop through and check if it matches each number
    for (int i = 0; i < d; i++)
    {
        for (int k = 0; k < d; k++)
        {
            if (i == d - 1 && k == d - 1)
                return true;
            if (board[i][k] != current_number)
                return false;
            current_number += 1;
        }
    }
    
    return true;
}
