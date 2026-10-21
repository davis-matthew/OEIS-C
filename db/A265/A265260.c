/*
 * A265260  Number of partitions of n into even squares.
 * https://oeis.org/A265260
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into even squares. For n not
 *            divisible by 4, a(n) = 0; otherwise a(n) = p_squares(n/4), where
 *            p_squares(m) counts partitions of m into squares (1^2, 2^2, 3^2,
 *            ...). The sequence is computed by a straightforward
 *            dynamic-programming sieve over square parts
 * Range:     u8 n <= 267, u16 n <= 831, u32 n <= 831, u64 n <= 831,
 *            u128 n <= 831, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *dp = NULL;          /* cached array of partitions into squares */
static idx_t dp_cap = 0;          /* allocated size of dp */

/* returns a(n) */
seq_t a(idx_t n)
{
    if (n % 4 != 0)                 /* no way to partition non‑multiples of 4 */
        return 0;

    idx_t m = n / 4;                /* reduce the problem to partitions into squares */

    /* rebuild dp if necessary */
    if (dp_cap <= m) {
        idx_t cap = m + 1;
        seq_t *new_dp = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
        if (!new_dp)               /* out of memory – abort (no alternative) */
            exit(EXIT_FAILURE);
        new_dp[0] = 1;              /* base case: empty partition of 0 */

        /* dynamic programming over square sizes */
        for (idx_t k = 1; k <= m / k; ++k) {   /* k*k <= m, avoids overflow */
            idx_t s = k * k;
            for (idx_t i = s; i <= m; ++i) {
                new_dp[i] = new_dp[i] + new_dp[i - s];
            }
        }

        free(dp);                  /* discard old cache */
        dp     = new_dp;
        dp_cap = cap;
    }

    return dp[m];
}
