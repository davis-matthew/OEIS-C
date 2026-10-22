/*
 * A392547  Expansion of 1 / ((1-x)^6 - x^7).
 * https://oeis.org/A392547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 31, u64 n <= 66, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A392547: Expansion of 1 / ((1‑x)^6 - x^7)
   Formulas in OEIS:
     a(n) = Sum_{k=0..⌊n/7⌋} binomial(n-k+5, n-7k)
     a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4)
            + 6*a(n-5) - a(n-6) + a(n-7)
   Initial values (a(0) through a(6)):
     1, 6, 21, 56, 126, 252, 462 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial seven values */
    static const seq_t init[7] = {
        (seq_t)1,  /* a(0) */
        (seq_t)6,  /* a(1) */
        (seq_t)21, /* a(2) */
        (seq_t)56, /* a(3) */
        (seq_t)126,/* a(4) */
        (seq_t)252,/* a(5) */
        (seq_t)462 /* a(6) */
    };

    /* Return directly for the first seven indices */
    if (n < 7)
        return init[(idx_t)n];

    /* Static cache for computed terms */
    static seq_t *cache   = NULL;
    static idx_t  cache_len = 0;

    /* Ensure the cache can hold at least one more term than the requested n */
    if (cache_len <= n) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;

        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;          /* allocation failure – unlikely for the tests */
        cache   = tmp;
        cache_len = new_len;

        /* Initialize the first seven terms if not already done */
        if (old_len == 0) {
            for (idx_t i = 0; i < 7; ++i)
                cache[i] = init[i];
        }

        /* Compute any missing terms from old_len up to n */
        for (idx_t i = old_len > 7 ? old_len : 7; i <= n; ++i) {
            seq_t pos = (seq_t)6 * cache[i-1]          // +6*a(n-1)
                    + (seq_t)20 * cache[i-3]          // +20*a(n-3)
                    + (seq_t)6 * cache[i-5]          // +6*a(n-5)
                    + cache[i-7];                    // + a(n-7)

            seq_t neg = (seq_t)15 * cache[i-2]          // -15*a(n-2)
                    + (seq_t)15 * cache[i-4]          // -15*a(n-4)
                    + cache[i-6];                    // - a(n-6)

            cache[i] = pos - neg;                      // compute a(i)
        }
    }

    return cache[n];
}
