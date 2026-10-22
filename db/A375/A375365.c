/*
 * A375365  Expansion of 1/( (1 + x)^2 * (1 - x^2*(1 + x)^3) ).
 * https://oeis.org/A375365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -2*a(n-1) + 5*a(n-3) + 10*a(n-4) + 10*a(n-5) + 5*a(n-6) +
 *            a(n-7) with initial values a(0)=1, a(1)=-2, a(2)=4, a(3)=-3,
 *            a(4)=6, a(5)=-2, a(6)=14
 * Range:     i8 n <= 10, i16 n <= 19, i32 n <= 36, i64 n <= 71, i128 n <= 141,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values */
static idx_t cache_size = 0U; /* current number of computed entries */

seq_t a(idx_t n)
{
    /* If we have not computed up to n, grow the cache */
    if (n >= cache_size) {
        idx_t new_size = n + 1U;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache)
            abort(); /* allocation failure */

        /* Compute newly required entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0U)      new_cache[0] = (seq_t)1;
            else if (i == 1U) new_cache[1] = (seq_t)-2;
            else if (i == 2U) new_cache[2] = (seq_t)4;
            else if (i == 3U) new_cache[3] = (seq_t)-3;
            else if (i == 4U) new_cache[4] = (seq_t)6;
            else if (i == 5U) new_cache[5] = (seq_t)-2;
            else if (i == 6U) new_cache[6] = (seq_t)14;
            else { /* i >= 7 */
                new_cache[i] =
                    ((seq_t)-2) * new_cache[i - 1U]
                    + ((seq_t)5) * new_cache[i - 3U]
                    + ((seq_t)10) * new_cache[i - 4U]
                    + ((seq_t)10) * new_cache[i - 5U]
                    + ((seq_t)5) * new_cache[i - 6U]
                    + new_cache[i - 7U];
            }
        }

        cache = new_cache;
        cache_size = new_size;
    }

    return cache[n];
}
