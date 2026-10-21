/*
 * A260024  Convert the last term from decimal to binary! a(1)=5.
 * https://oeis.org/A260024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1) = 5; for n > 1, a(n) = decimal( binary( a(n‑1) ) )
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoization */
static seq_t *cache   = NULL;   /* array of computed terms */
static idx_t  cached_max = 0;   /* largest n already computed */
static size_t cache_cap  = 0;   /* allocated capacity (in terms) */

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n < 1 */

    /* initialize cache on first call */
    if (cache == NULL) {
        size_t init_cap = 16;
        cache = (seq_t *)malloc(init_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)5;                  /* a(1) */
        cached_max = 1;
        cache_cap  = init_cap;
    }

    /* return cached value if already known */
    if (n <= cached_max) return cache[(size_t)(n - 1)];

    /* grow cache if necessary */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t i = cached_max + (idx_t)1; i <= n; ++i) {
        seq_t prev = cache[(size_t)(i - (idx_t)2)];   /* a(i‑1) */
        seq_t result = (seq_t)0;

        /* find the highest power of two <= prev */
        seq_t pow2 = (seq_t)1;
        if (prev > (seq_t)0) {
            while (pow2 <= prev) {
                /* avoid shifting beyond the type width */
                if (pow2 > (prev >> (idx_t)1)) break;
                pow2 <<= (idx_t)1;
            }

            /* process bits from most significant to least */
            seq_t rem = prev;
            while (pow2 > (seq_t)0) {
                result = result * (seq_t)10;
                if (rem >= pow2) {
                    result += (seq_t)1;
                    rem -= pow2;
                }
                pow2 >>= (idx_t)1;
            }
        }

        cache[(size_t)(i - (idx_t)1)] = result;   /* store a(i) */
    }

    cached_max = n;
    return cache[(size_t)(n - (idx_t)1)];
}
