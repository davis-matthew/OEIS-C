/*
 * A341679  a(1) = 1; for n > 1, a(n) = n divided by the most recently
 *          appearing divisor of n in all previous terms.
 * https://oeis.org/A341679
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n divided by the most recently appearing divisor of n in
 *            all previous terms (OEIS A341679; a(1)=1)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* seq_t cache[0..max]  (cache[0] unused) */
static idx_t  cache_size = 0; /* number of computed terms in cache */

/* Ensure the cache contains values up to n (inclusive);
   allocate or grow as necessary, compute new terms. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;          /* already computed */

    if (cache == NULL) {
        /* first allocation */
        cache = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = 0;                     /* unused placeholder */
        cache[1] = 1;                     /* a(1) = 1 */
        cache_size = 1;
    } else {
        /* grow existing array */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* compute new terms from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val = (seq_t)i;
        seq_t most_recent_divisor = 1; /* default divisor (a(1)) */

        /* find the most recent prior term that divides val */
        for (idx_t k = i - 1; k >= 1; --k) {
            if (val % cache[k] == 0) {
                most_recent_divisor = cache[k];
                break;
            }
            if (k == 1) break; /* avoid unsigned wrap */
        }

        cache[i] = val / most_recent_divisor;
        if (i == n) break; /* avoid extra iterations when i==n in loop */
    }

    cache_size = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined; guard against bad input */

    ensure_cache(n);       /* guarantees cache[n] computed */

    return cache[n];
}
