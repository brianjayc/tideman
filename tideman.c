#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool will_not_cycle(int winner, int loser);
int get_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO - my solution below
    // check if name is in candidates and if found add candidate index to ranks by using the supplied rank preference
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO - my solution below

    for (int i = 0; i < candidate_count - 1; i++) // iterate for voters candidate preference and add it to global preferences count
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO - my solution below
    // iterate over preferences if value is more than it reverse pair then add winner and loser to pairs. lastly update pair count
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // TODO - my solution below, sorting using bubble sort
    int swap = -1;

    while (swap != 0)
    {
        swap = 0;

        for (int i = 0; i < pair_count - 1; i++)
        {
            // check if current pair is weaker than the next pair. if weaker switch it
            if (preferences[pairs[i].winner][pairs[i].loser] < preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                int w = pairs[i + 1].winner;
                int l = pairs[i + 1].loser;
                pairs[i + 1].winner = pairs[i].winner;
                pairs[i + 1].loser = pairs[i].loser;
                pairs[i].winner = w;
                pairs[i].loser = l;
                swap += 1;
            }
        }
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO - my solution below

    for (int i = 0; i < pair_count; i++) //iterate over the pairs
    {
        if (will_not_cycle(pairs[i].winner, pairs[i].loser)) //check if winner and loser will not create a cycle
        {
            locked[pairs[i].winner][pairs[i].loser] = true; // lock pairs if it will not create cycle
        }
    }

    return;
}

bool will_not_cycle(int winner, int loser) // recursive function to check if it will not cycle
{
    if (winner == loser) // check if its a cycle if winner equals loser
    {
        return false; // return false if its a cycle
    }

    for (int i = 0; i < candidate_count; i++) // iterate over all possible pairs of the loser
    {
        if (locked[loser][i]) // if loser and its pair are locked
        {
            if (will_not_cycle(winner, i) == false) // recursively call the function again to check if it will not cycle
            {
                return false; // return false if its a cycle
            }
        }
    }

    return true; // return true after iterating over all possible loser pair combo, true in a sense that winner and loser will not cycle
}

// Print the winner of the election
void print_winner(void)
{
    // TODO - my solution below
    printf("%s\n", candidates[get_winner()]);
    return;
}

int get_winner(void)
{
    int winner = -1; // create winner variable set it initially to -1 in case no winners

    for (int i = 0; i < candidate_count; i++) // iterate over all candidates
    {
        // create is locked variable as a placeholder if current candidate has been locked by other winning candidates
        bool is_locked = false;

        for (int j = 0; j < candidate_count; j++) // check current candidate if has been locked by other winning candidates
        {
            if (locked[j][i]) // check if has been locked
            {
                is_locked = true; // if yes set is locked to true
                break; // break to stop further probing since current candidate has been locked by other winning candidates
            }
        }

        // check is locked variable if current candidate has not been locked if yes set winner to i (current candidate)
        if (is_locked == false)
        {
            winner = i;
            break; // break for loop since we have  found the winner that has not been locked by other winning candidates
        }
    }

    return winner;
}
