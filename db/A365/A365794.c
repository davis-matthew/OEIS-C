/*
 * A365794  Expansion of e.g.f. 1 / (3 - 2 * exp(2*x))^(3/4).
 * https://oeis.org/A365794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   pow2, seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A365794: Expansion of e.g.f. 1 / (3 - 2 * exp(2*x))^(3/4).
   Recurrence:
       a(0) = 1
       a(n) = 2 * sum_{j=1}^{n-1} C(n-1, j-1) * 2^{n-j} * a(j)
              + 3 * sum_{j=0}^{n-1} C(n-1, j)   * 2^{n-1-j} * a(j)
   This recurrence uses only previously computed terms.
*/

#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/pow2.h"

/* Returns a(n) for the sequence A365794. */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;            /* number of terms stored (next free index) */

    if (n >= cache_len) {               /* need to grow the cache */
        idx_t new_len = n + 1;
        seq_t *new_cache = seq_alloc_new_array(new_len);

        /* copy existing terms if cache was previously allocated */
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_len; ++i)
                new_cache[i] = cache[i];
        }

        /* initialise a(0) if it wasn't yet set */
        if (cache_len == 0)
            new_cache[0] = 1;

        /* compute missing terms */
        for (idx_t k = cache_len; k <= n; ++k) {
            if (k == 0) continue;     /* a(0) already set */

            seq_t S1 = 0;              /* sum for the first part of the recurrence */
            seq_t S2 = 0;              /* sum for the second part of the recurrence */

            seq_t binom1 = 1;          /* C(k-1, 0) */
            seq_t binom2 = 1;          /* C(k-1, 0) */
            seq_t power1 = pow2(k - 1); /* 2^(k-1) */
            seq_t power2 = pow2(k - 1); /* 2^(k-1) */

            /* Sum over j = 1 .. k-1 for the first term (S1) */
            for (idx_t j = 1; j < k; ++j) {
                seq_t term1 = binom1 * power1 * new_cache[j];
                S1 += term1;

                /* Update binomial coefficient C(k-1, j) for next j */
                binom1 = binom1 * (seq_t)(k - j) / (seq_t)(j);
                /* Halve the power for the next iteration */
                power1 = power1 / 2;
            }

            /* Sum over j = 0 .. k-1 for the second term (S2) */
            for (idx_t j = 0; j < k; ++j) {
                seq_t term2 = binom2 * power2 * new_cache[j];
                S2 += term2;

                /* Update binomial coefficient C(k-1, j+1) for next j */
                binom2 = binom2 * (seq_t)(k - j - 1) / (seq_t)(j + 1);
                /* Halve the power for the next iteration */
                power2 = power2 / 2;
            }

            /* Assemble the recurrence */
            new_cache[k] = 2 * S1 + 3 * S2;
        }

        /* Replace old cache with the expanded one */
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[n];
}
