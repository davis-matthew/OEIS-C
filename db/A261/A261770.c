/*
 * A261770  Expansion of Product_{k>=1} (1 + x^k) / (1 + x^(6*k)).
 * https://oeis.org/A261770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts where no
 *            part is divisible by 6. This is computed by the classic
 *            dynamic‑programming method for the generating function
 *            ∏_{k≥1,k%6≠0} (1 + x^k)
 * Range:     u8 n <= 33, u16 n <= 92, u32 n <= 281, u64 n <= 943,
 *            u128 n <= 3374, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;   /* dp[i] holds a(i) for i ≤ max_n */
    static idx_t max_n = 0;    /* largest n for which dp has been built */

    /* If dp is uninitialised or the requested index exceeds what
       has been built, rebuild the array up to the new size. */
    if (dp == NULL || n > max_n) {
        /* Allocate (or grow) the array.  Use realloc so that previous
           values are preserved when the size does not change. */
        size_t new_size = (size_t)n + 1;
        seq_t *new_dp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
        if (!new_dp)
            abort();                 /* allocation failed */
        dp = new_dp;

        /* Initialise entries to zero. */
        for (size_t i = 0; i <= (size_t)n; ++i)
            dp[i] = 0;
        dp[0] = 1;                    /* empty partition */

        /* Dynamic programming over distinct parts, skipping multiples of 6. */
        for (idx_t part = 1; part <= n; ++part) {
            if (part % 6 == 0)
                continue;             /* skip forbidden parts */
            for (idx_t sum = n; sum >= part; --sum) {
                dp[sum] += dp[sum - part];
            }
        }
        max_n = n;
    }

    return dp[n];
}
