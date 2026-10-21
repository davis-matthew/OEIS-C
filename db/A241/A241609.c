/*
 * A241609  Number of length n+2 0..3 arrays with no consecutive three
 *          elements summing to more than 3.
 * https://oeis.org/A241609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A241609
   a(n) = number of length (n+2) arrays over {0,1,2,3} such that
   no three consecutive elements sum to more than 3.
   Computed by dynamic programming over the last two digits.
   Initial states: all 16 pairs of digits with count 1.
   Transition: for each state (x,y) add digit z if x+y+z <= 3,
   producing new state (y,z).
   Run this transition n times to obtain sequences of length n+2.
   Complexity: O(n) with a small constant. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Return 0 for non-positive n; sequence is defined for n >= 1. */
    if (n < 1)
        return (seq_t)0;

    /* DP arrays: counts for each pair (x,y). 4x4 = 16 states. */
    seq_t cur[4][4] = {0};
    seq_t nxt[4][4] = {0};

    /* Initial counts for all 16 first pairs. */
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y)
            cur[x][y] = (seq_t)1;

    /* Perform n transitions: each adds one digit,
       building sequences of length n+2. */
    for (idx_t step = 0; step < n; ++step)
    {
        /* Reset next array to zero. */
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                nxt[i][j] = (seq_t)0;

        /* Transition: from (x,y) to (y,z) if allowed. */
        for (int x = 0; x < 4; ++x)
        {
            for (int y = 0; y < 4; ++y)
            {
                seq_t cnt = cur[x][y];
                if (cnt == (seq_t)0) continue;
                for (int z = 0; z < 4; ++z)
                {
                    if ((seq_t)(x + y + z) <= (seq_t)3)
                        nxt[y][z] += cnt;
                }
            }
        }

        /* Copy next to current for next iteration. */
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                cur[i][j] = nxt[i][j];
    }

    /* Sum all counts for sequences of the final length. */
    seq_t total = (seq_t)0;
    for (int x = 0; x < 4; ++x)
        for (int y = 0; y < 4; ++y)
            total += cur[x][y];

    return total;
}
