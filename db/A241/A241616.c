/*
 * A241616  Number of length n+2 0..10 arrays with no consecutive three
 *          elements summing to more than 10.
 * https://oeis.org/A241616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) = number of arrays of length n+2 with entries in 0..10 such that
   for every i, a_i + a_{i+1} + a_{i+2} ≤ 10.
   The algorithm counts sequences by dynamic programming over the last two
   values of the array.  No explicit combinatorial formula is used.
*/
#include <stdbool.h>

seq_t a(idx_t n)
{
    const int V = 11;                   /* values 0..10 */
    const int MAX_V = 10;               /* maximum value for sum constraint elimination */

    /* length of the array to count: L = n + 2 */
    idx_t L = n + 2;
    if (L < 3)          /* n < 1 is undefined, but return 0 for safety */
        return 0;

    /* DP tables: dp[i][j] = number of sequences of current length
       ending with last two values i and j.   i, j ∈ 0..10 */
    seq_t dp[V][V];
    seq_t ndp[V][V];

    /* initialize for length 2: all pairs are allowed (no constraint yet) */
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            dp[i][j] = 1;

    /* extend the sequences to length L */
    for (idx_t pos = 3; pos <= L; ++pos) {
        /* zero next table */
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                ndp[i][j] = 0;

        /* transition */
        for (int i = 0; i < V; ++i) {
            for (int j = 0; j < V; ++j) {
                seq_t val = dp[i][j];
                if (!val)
                    continue;
                int maxz = MAX_V - i - j;
                if (maxz < 0)
                    continue;
                for (int z = 0; z <= maxz; ++z)
                    ndp[j][z] += val;
            }
        }

        /* copy new table back to dp for next iteration */
        for (int i = 0; i < V; ++i)
            for (int j = 0; j < V; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* result is sum over all possible last two values */
    seq_t total = 0;
    for (int i = 0; i < V; ++i)
        for (int j = 0; j < V; ++j)
            total += dp[i][j];

    return total;
}
