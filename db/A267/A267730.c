/*
 * A267730  Number of nX3 0..1 arrays with every repeated value in every row
 *          and column greater than or equal to the previous repeated value.
 * https://oeis.org/A267730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A267730 – Number of n×3 0..1 arrays with non‑decreasing repeated values.
 *
 *  Let c(n) be the number of binary strings of length n in which the
 *  value that is repeated (a run of length ≥2) never changes from 1 to 0
 *  as we scan the string.  The desired sequence is
 *
 *      a(n) = c(n)^3
 *
 *  Because the three columns are independent and each column must satisfy
 *  the same property, the total number of arrays is the cube of the
 *  number of admissible columns.
 *
 *  c(n) is computed by a very small DP that keeps track of:
 *      - last bit written (0 or 1)
 *      - whether the current run has already reached length ≥2
 *      - whether a repeated 1 has occurred earlier
 *  The transition forbids creating a repeated 0 after a repeated 1.
 *  The DP runs in O(n) time and uses only 8 states, so it easily fits
 *  the required time limits for n up to 10 000.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* undefined for offset<1, return 0 */

    /* state[ last_bit ][ run_ge2 ][ seen_rep1 ]  */
    seq_t dp[2][2][2] = {0};
    seq_t ndp[2][2][2] = {0};

    /* first bit */
    dp[0][0][0] = (seq_t)1;
    dp[1][0][0] = (seq_t)1;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* reset ndp */
        for (int last = 0; last < 2; ++last)
            for (int run = 0; run < 2; ++run)
                for (int seen = 0; seen < 2; ++seen)
                    ndp[last][run][seen] = (seq_t)0;

        for (int last = 0; last < 2; ++last)
            for (int run = 0; run < 2; ++run)
                for (int seen = 0; seen < 2; ++seen)
                {
                    seq_t cnt = dp[last][run][seen];
                    if ((seq_t)0 == cnt)
                        continue;

                    for (int bit = 0; bit < 2; ++bit)
                    {
                        /* forbid a repeated 0 after a repeated 1 */
                        if (bit == last && bit == 0 && run == 0 && seen == 1)
                            continue;

                        int   new_last = bit;
                        int   new_run = (bit == last) ? 1 : 0;
                        int   new_seen = seen;

                        if (bit == last && bit == 1 && run == 0)
                            new_seen = 1;          /* a repeated 1 appears */

                        ndp[new_last][new_run][new_seen] += cnt;
                    }
                }

        /* copy ndp to dp for next iteration */
        for (int last = 0; last < 2; ++last)
            for (int run = 0; run < 2; ++run)
                for (int seen = 0; seen < 2; ++seen)
                    dp[last][run][seen] = ndp[last][run][seen];
    }

    /* sum all states to get c(n) */
    seq_t c = (seq_t)0;
    for (int last = 0; last < 2; ++last)
        for (int run = 0; run < 2; ++run)
            for (int seen = 0; seen < 2; ++seen)
                c += dp[last][run][seen];

    /* a(n) = c^3 */
    seq_t a = c * c;
    a = a * c;
    return a;
}
