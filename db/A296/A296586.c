/*
 * A296586  Number of nX6 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0, 2 or 3 neighboring 1s.
 * https://oeis.org/A296586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of n×6 0..1 arrays such that each 1 has 0, 2 or 3
 *            neighbors among the 6 directions (horiz., vert., up‑right,
 *            down‑left)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

#define WIDTH 6
#define PAT_COUNT 64
#define STATE_COUNT (PAT_COUNT * PAT_COUNT)

/* Transition table: for each pair of previous rows (a,b)
   store a 64‑bit mask of all rows c that are allowed next. */
static uint64_t transitions[STATE_COUNT];
static bool initialized = false;

/* Check if a given row b satisfies the degree condition
   when surrounded by rows a (above) and c (below). */
static bool check_transition(unsigned a, unsigned b, unsigned c)
{
    for (unsigned j = 0; j < WIDTH; ++j)
    {
        if (!(b & (1U << j)))
            continue;                      /* only 1's are relevant */

        unsigned cnt = 0;                  /* number of neighboring 1s */

        /* horizontal neighbours in the same row */
        if (j > 0   && (b & (1U << (j-1)))) ++cnt;
        if (j + 1 < WIDTH && (b & (1U << (j+1)))) ++cnt;

        /* vertical neighbours */
        if (a & (1U << j)) ++cnt;
        if (c & (1U << j)) ++cnt;

        /* anti‑diagonal neighbours */
        if (j + 1 < WIDTH && (a & (1U << (j+1)))) ++cnt;
        if (j > 0   && (c & (1U << (j-1)))) ++cnt;

        if (cnt == 1 || cnt > 3)           /* forbidden counts */
            return false;
    }
    return true;
}

/* Build the transition table once. */
static void init_transitions(void)
{
    if (initialized) return;
    for (unsigned a = 0; a < PAT_COUNT; ++a)
    {
        for (unsigned b = 0; b < PAT_COUNT; ++b)
        {
            uint64_t mask = 0;
            for (unsigned c = 0; c < PAT_COUNT; ++c)
                if (check_transition(a, b, c))
                    mask |= ((uint64_t)1U << c);
            transitions[a * PAT_COUNT + b] = mask;
        }
    }
    initialized = true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    init_transitions();

    /* DP arrays: count of ways leading to each state (a,b). */
    static seq_t dp[STATE_COUNT];
    static seq_t newdp[STATE_COUNT];

    /* Initialise with two empty rows (0,0). */
    memset(dp, 0, sizeof(dp));
    dp[0] = (seq_t)1;               /* state index 0 corresponds to (0,0) */

    for (idx_t row = 0; row < n; ++row)
    {
        memset(newdp, 0, sizeof(newdp));

        for (unsigned s = 0; s < STATE_COUNT; ++s)
        {
            seq_t val = dp[s];
            if (val == 0) continue;

            uint64_t mask = transitions[s];
            if (mask == 0) continue;

            unsigned curr = (unsigned)(s % PAT_COUNT);   /* previous row (b) */

            /* iterate over allowed next rows (c) */
            while (mask)
            {
                unsigned c = __builtin_ctzll(mask);
                mask &= mask - 1;

                unsigned new_state = curr * PAT_COUNT + c;
                newdp[new_state] += val;
            }
        }

        memcpy(dp, newdp, sizeof(dp));
    }

    /* Final dummy row of zeros (index 0) must be appended. */
    seq_t ans = 0;
    for (unsigned s = 0; s < STATE_COUNT; ++s)
    {
        if (dp[s] != 0 && (transitions[s] & 1ULL))
            ans += dp[s];
    }

    return ans;
}
