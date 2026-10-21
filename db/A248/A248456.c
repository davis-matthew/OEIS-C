/*
 * A248456  Number of length n+2 0..3 arrays with no three consecutive terms
 *          having the sum of any two elements equal to twice the third.
 * https://oeis.org/A248456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 17, u64 n <= 36, u128 n <= 75,
 *            big any n
 * Helpers:   bad_triple
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) by dynamic programming over sequences of length n+2 with no forbidden triple */
#include <stdbool.h>
#include "../utils/bad_triple.h"

seq_t a(idx_t n)
{
    /* length of the array is n+2 */
    idx_t N = n + 2;

    /* dp_prev and dp_curr hold counts for sequences ending with last two elements (a,b) */
    seq_t dp_prev[4][4];
    seq_t dp_curr[4][4];

    /* initialize for length 2: all 4^2 combinations are allowed */
    for (idx_t i = 0; i < 4; ++i)
        for (idx_t j = 0; j < 4; ++j)
            dp_prev[i][j] = 1;

    /* iterate positions 2 .. N-1 to extend sequences to length N */
    for (idx_t pos = 2; pos < N; ++pos)
    {
        /* zero dp_curr */
        for (idx_t i = 0; i < 4; ++i)
            for (idx_t j = 0; j < 4; ++j)
                dp_curr[i][j] = 0;

        /* transition: add a new element c to each sequence ending with (a,b) */
        for (idx_t a = 0; a < 4; ++a)
        {
            for (idx_t b = 0; b < 4; ++b)
            {
                seq_t val = dp_prev[a][b];
                if (val == 0)
                    continue;
                for (idx_t c = 0; c < 4; ++c)
                    if (!bad_triple((int)a, (int)b, (int)c))
                        dp_curr[b][c] += val;
            }
        }

        /* copy dp_curr to dp_prev for next iteration */
        for (idx_t i = 0; i < 4; ++i)
            for (idx_t j = 0; j < 4; ++j)
                dp_prev[i][j] = dp_curr[i][j];
    }

    /* sum all sequences of length N */
    seq_t res = 0;
    for (idx_t i = 0; i < 4; ++i)
        for (idx_t j = 0; j < 4; ++j)
            res += dp_prev[i][j];

    return res;
}
