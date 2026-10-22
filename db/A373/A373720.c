/*
 * A373720  Expansion of e.g.f. exp(x * (1 + x^2)^3).
 * https://oeis.org/A373720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 9*(n-1)*(n-2)*a(n-3) +
 *            15*(n-1)*(n-2)*(n-3)*(n-4)*a(n-5) +
 *            7*(n-1)*(n-2)*(n-3)*(n-4)*(n-5)*(n-6)*a(n-7), with a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=19, a(4)=73, a(5)=541, a(6)=5761
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_len = 0;   /* first index not yet computed */

    /* Base values for 0..6 */
    static const seq_t base_vals[7] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)19,
        (seq_t)73, (seq_t)541, (seq_t)5761
    };

    /* Ensure the cache array is large enough for index n */
    if (cache_len <= n) {
        size_t new_len = (cache_len == 0) ? 8 : cache_len * 2;
        while (new_len <= (size_t)n) new_len <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache   = tmp;
        cache_len = (idx_t)new_len;
    }

    /* Initialise the base entries 0..6 once */
    if (computed_len < 7) {
        for (idx_t i = computed_len; i < 7; ++i) {
            cache[i] = base_vals[i];
        }
        computed_len = 7;   /* all base entries have been written */
    }

    /* Compute values up to n if required */
    if (n >= computed_len) {
        for (idx_t i = computed_len; i <= n; ++i) {
            seq_t val = cache[i-1];   /* a(i-1) is always available */

            if (i >= 3) {
                seq_t p = (seq_t)9
                        * ((seq_t)(i-1))
                        * ((seq_t)(i-2))
                        * cache[i-3];
                val += p;
            }
            if (i >= 5) {
                seq_t p = (seq_t)15
                        * ((seq_t)(i-1))
                        * ((seq_t)(i-2))
                        * ((seq_t)(i-3))
                        * ((seq_t)(i-4))
                        * cache[i-5];
                val += p;
            }
            if (i >= 7) {
                seq_t p = (seq_t)7
                        * ((seq_t)(i-1))
                        * ((seq_t)(i-2))
                        * ((seq_t)(i-3))
                        * ((seq_t)(i-4))
                        * ((seq_t)(i-5))
                        * ((seq_t)(i-6))
                        * cache[i-7];
                val += p;
            }
            cache[i] = val;
        }
        computed_len = n + 1;
    }

    return cache[n];
}
