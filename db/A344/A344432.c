/*
 * A344432  a(n) = Sum_{k=1..n} mu(k) * 2^(n - k).
 * https://oeis.org/A344432
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(n) = 2*a(n-1) + mu(n) (n > 0)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   mu_of, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>          /* for size_t */
#include "../utils/mu_of.h"  /* int mu_of(idx_t) */
#include "../utils/seq_realloc.h" /* seq_t* seq_realloc(seq_t*, size_t, size_t) */

seq_t a(idx_t n)
{
    /* Memoisation cache */
    static seq_t *cache = NULL;     /* array of computed values */
    static idx_t max_computed = 0;  /* highest index stored in cache */

    /* Initialise cache for a(0) if it doesn't exist yet */
    if (cache == NULL) {
        cache = seq_realloc(NULL, 0, 1);  /* allocate space for a(0) */
        if (cache == NULL) return 0;      /* allocation failure (unlikely) */
        cache[0] = 0;                    /* a(0) = 0 */
        max_computed = 0;
    }

    /* If we need higher values, extend the cache */
    if (n > max_computed) {
        seq_t *tmp = seq_realloc(cache, max_computed + 1, n + 1);
        if (tmp == NULL) return 0;       /* allocation failure */
        cache = tmp;

        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t val = cache[i - 1] << 1;   /* 2 * a(i-1) */
            int mu = mu_of(i);               /* Möbius value (-1, 0 or 1) */
            val += (seq_t)mu;                /* unsigned addition works for -1 */
            cache[i] = val;
        }
        max_computed = n;
    }

    return cache[n];
}
