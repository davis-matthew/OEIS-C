/*
 * A262474  Number of (4+1)X(n+1) 0..1 arrays with each row divisible by 3 and
 *          each column divisible by 5, read as a binary number with top and
 *          left being the most significant bits.
 * https://oeis.org/A262474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×(n+1) binary arrays with each row interpreted
 *            as a binary number divisible by 3 and each column divisible by
 *            5, where top and left are the most significant bits.
 *            Computation: dynamic programming over columns, keeping the
 *            remainder modulo 3 of each of the 5 rows. Transition matr...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */
    const idx_t N = n + 1;         /* number of columns */

    /* 7 column patterns whose 5‑bit value is divisible by 5 */
    static const idx_t col_patterns[7] = {0, 5, 10, 15, 20, 25, 30};

    /* transition table: trans[state][p] -> new_state (0–242) */
    static bool trans_inited = false;
    static unsigned char trans[243][7];
    if (!trans_inited) {
        for (int s = 0; s < 243; ++s) {
            /* decode state into remainders r[0..4] (base‑3 digits) */
            int rem[5];
            int tmp = s;
            for (int i = 0; i < 5; ++i) {
                rem[i] = tmp % 3;
                tmp /= 3;
            }
            for (int p = 0; p < 7; ++p) {
                int base = 1;
                int ns   = 0;
                for (int i = 0; i < 5; ++i) {
                    int bit = (col_patterns[p] >> (4 - i)) & 1;
                    int new_rem = (rem[i] * 2 + bit) % 3;
                    ns += new_rem * base;
                    base *= 3;
                }
                trans[s][p] = (unsigned char)ns;
            }
        }
        trans_inited = true;
    }

    /* DP arrays */
    static seq_t dp[243];
    static seq_t ndp[243];
    for (int i = 0; i < 243; ++i) dp[i] = 0;
    dp[0] = 1;   /* all remainders zero before any column */

    for (idx_t step = 0; step < N; ++step) {
        for (int i = 0; i < 243; ++i) ndp[i] = 0;
        for (int s = 0; s < 243; ++s) {
            if (dp[s] == 0) continue;
            seq_t cur = dp[s];
            for (int p = 0; p < 7; ++p) {
                ndp[ trans[s][p] ] += cur;
            }
        }
        /* copy ndp back into dp for next iteration */
        for (int i = 0; i < 243; ++i) dp[i] = ndp[i];
    }

    return dp[0];   /* all rows divisible by 3 */
}
