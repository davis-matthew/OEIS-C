/*
 * A339014  E.g.f.: exp(2 * (exp(x) - 1 - x - x^2 / 2)).
 * https://oeis.org/A339014
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(1) = 0; a(2) = 0; a(n) = 2 * Σ_{k=3..n}
 *            binomial(n-1, k-1) * a(n-k) (n ≥ 3) The definition is
 *            implemented via dynamic programming with caching
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 26, u128 n <= 42,
 *            big any n
 * Helpers:   binomial, ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The required utility functions are in the utils directory.  */
/*  Note: all index operations use idx_t (unsigned), and all value
   operations are performed with seq_t. */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* cached values of the sequence */
    static seq_t *cache = NULL;       /* pointer to array of seq_t */
    static size_t cache_cap = 0;      /* number of allocated slots */
    static size_t cache_len = 0;      /* number of computed entries */

    /* make sure the array can hold up to n */
    /* ensure_cache will enlarge the allocation if needed. */
    if (n + 1 > cache_cap)
        ensure_cache(n + 1, &cache, &cache_cap);

    /* compute missing entries up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1 || i == 2) {
            cache[i] = (seq_t)0;
        } else {
            seq_t sum = (seq_t)0;
            /* j corresponds to the index in the recurrence: a(i) depends on
               a(j) where j = 0 .. i-3. */
            for (idx_t j = 0; j <= i - 3; ++j) {
                seq_t term = binomial(i - 1, j); /* binomial(n-1, j) */
                term *= cache[j];                /* term = binomial * a(j) */
                sum += term;                     /* accumulate */
            }
            cache[i] = sum * (seq_t)2; /* multiply by 2 */
        }
    }
    cache_len = n + 1; /* all entries up to n are now valid */

    return cache[n];
}
