/*
 * A251562  Number of (n+1)X(1+1) 0..7 arrays with every 2X2 subblock summing
 *          to a prime.
 * https://oeis.org/A251562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = # of (n+1)×2 arrays of digits 0..7 such that each 2×2
 *            subblock sums to a prime. Computed by dynamic programming on the
 *            64 possible row states
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset; the specification guarantees n≥1. */
    if (n < (idx_t)1)
        return (seq_t)0;

    /* Precompute the transition table once. */
    static bool ready = false;
    static unsigned char sum_table[64][64];
    static bool prime_table[64][64];
    if (!ready) {
        for (int s = 0; s < 64; ++s) {
            int a0 = s / 8;
            int a1 = s % 8;
            for (int t = 0; t < 64; ++t) {
                int b0 = t / 8;
                int b1 = t % 8;
                int sum = a0 + a1 + b0 + b1;
                sum_table[s][t] = (unsigned char)sum;
                /* primality table for 0..28 */
                prime_table[s][t] =
                    (sum >= 2) && (sum == 2 || sum == 3 || sum == 5 || sum == 7 ||
                                   sum == 11 || sum == 13 || sum == 17 || sum == 19 ||
                                   sum == 23);
            }
        }
        ready = true;
    }

    /* DP arrays for current and next row counts. */
    seq_t dp_cur[64];
    seq_t dp_nxt[64];
    for (int i = 0; i < 64; ++i)
        dp_cur[i] = (seq_t)1;            /* initial row: any of 64 states */

    /* Walk through n transitions (n steps produce n+1 rows). */
    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 64; ++i)
            dp_nxt[i] = (seq_t)0;        /* clear next array */

        for (int cur = 0; cur < 64; ++cur) {
            if (dp_cur[cur] == (seq_t)0)
                continue;
            seq_t val = dp_cur[cur];
            for (int nxt = 0; nxt < 64; ++nxt) {
                if (prime_table[cur][nxt])
                    dp_nxt[nxt] += val;
            }
        }
        /* copy next to current for next iteration */
        for (int i = 0; i < 64; ++i)
            dp_cur[i] = dp_nxt[i];
    }

    /* Sum over all final states to obtain total number of arrays. */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 64; ++i)
        result += dp_cur[i];

    return result;
}
