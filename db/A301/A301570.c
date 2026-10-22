/*
 * A301570  Expansion of Product_{k>=1} (1 + x^(5*k))*(1 + x^(5*k-1)).
 * https://oeis.org/A301570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts congruent
 *            to 0 or 4 mod 5 (OEIS A301570)
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *dp = NULL;      /* DP table: dp[i] = number of partitions of i */
    static idx_t dp_max = 0;      /* highest n that has been computed */
    static size_t dp_cap = 0;     /* current allocated capacity of dp */

    /* If we have never initialized or need to grow to a larger n, recompute. */
    if (dp == NULL || n > dp_max) {
        idx_t required = (n > dp_max) ? n : dp_max;
        size_t new_cap = (size_t)(required + 1);

        /* Allocate or resize the table */
        seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failure */
        dp = tmp;
        /* Zero our newly allocated portion if we enlarged the table */
        if (new_cap > dp_cap)
            memset(dp + dp_cap, 0, (new_cap - dp_cap) * sizeof(seq_t));
        dp_cap = new_cap;

        /* Compute the DP table from scratch up to 'required'. */
        for (idx_t i = 0; i <= required; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* Allowed parts: numbers ≡ 0 or 4 (mod 5) */
        for (idx_t part = 4; part <= required; ++part) {
            if ((part % 5) == 0 || (part % 5) == 4) {
                for (idx_t sum = required; sum >= part; --sum)
                    dp[sum] += dp[sum - part];
            }
        }

        dp_max = required;
    }

    return dp[n];
}
