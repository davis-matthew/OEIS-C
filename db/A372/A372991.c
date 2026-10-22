/*
 * A372991  a(n) = (2n)!/(a(n-1)*a(n-2)), where a(0)=1, a(1) = 1.
 * https://oeis.org/A372991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n*(2*n-1)*a(n-3) (n >= 3) with initial values a(0) = 1,
 *            a(1) = 1, a(2) = 24
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*--------------------------------------------*/
#include <stdlib.h>
#include <stddef.h>

/* Static cache for computed values */
static seq_t *seq_cache = NULL;   /* array of a(n) */
static size_t cache_cap = 0;      /* current capacity of seq_cache   */
static idx_t  cache_max = 0;      /* highest index computed so far   */

/*--------------------------------------------*/
seq_t a(idx_t n)
{
    /* Initialise cache on first call */
    if (!seq_cache) {
        cache_cap = 4;                         /* start with small capacity */
        seq_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!seq_cache) return 0;              /* allocation failure */

        /* base cases */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)24;
        cache_max = 2;
    }

    /* Return cached value if already computed */
    if (n <= cache_max) {
        return seq_cache[(size_t)n];
    }

    /* Grow cache to accommodate index n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while ((size_t)n >= new_cap) new_cap *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_cache) return 0;              /* allocation failure */

        /* Copy existing values */
        for (size_t i = 0; i < cache_cap; ++i) {
            new_cache[i] = seq_cache[i];
        }
        free(seq_cache);
        seq_cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (idx_t k = cache_max + 1; k <= n; ++k) {
        seq_t mult = (seq_t)(2 * k) * ( (seq_t)(2 * k) - (seq_t)1 );
        /* k >= 3 -> k-3 is valid */
        seq_cache[(size_t)k] = mult * seq_cache[(size_t)(k - 3)];
    }
    cache_max = n;
    return seq_cache[(size_t)n];
}
