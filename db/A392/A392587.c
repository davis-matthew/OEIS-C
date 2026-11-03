/*
 * A392587  Expansion of 1 / ((1-x)^3 - x^4)^2.
 * https://oeis.org/A392587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/4⌋} (k+1) * binomial(n‑k+5, n‑4k)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 30, u64 n <= 65, u128 n <= 138,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 947 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

static seq_t *cache = NULL;          /* memoisation cache */
static idx_t  cache_size = 0;        /* current cache size (number of stored terms) */

/* Pre‑defined first eight values (used implicitly by the binomial sum as well) */
static const seq_t init[8] = {
    (seq_t)1,  (seq_t)6,  (seq_t)21,  (seq_t)56,
    (seq_t)128, (seq_t)270, (seq_t)552, (seq_t)1122
};

/* Extend the cache up to at least index n.  New entries are filled by the
   binomial formula, which automatically gives the correct values for
   n < 8 as well, but for clarity we use the explicit init table there. */
static void ensure_cache(idx_t n)
{
    if (cache != NULL && n < cache_size)
        return;                                       /* nothing to do */

    idx_t new_size = n + 1;
    seq_t *new_cache = NULL;

    if (cache == NULL) {                               /* first allocation */
        new_cache = mem_new(seq_t, new_size);
        if (!new_cache)
            return;                                   /* allocation failure */
        cache_size = 0;                                /* will be updated below */
    } else {                                           /* growth */
        new_cache = mem_resize(seq_t, cache, new_size);
        if (!new_cache)
            return;                                   /* allocation failure */
    }

    cache = new_cache;

    /* Compute each new entry from cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 8) {                                   /* copy the base line */
            cache[i] = init[i];
        } else {
            seq_t sum = 0;
            idx_t maxk = i / 4;
            for (idx_t k = 0; k <= maxk; ++k) {
                seq_t bin = binomial(i - k + 5, i - 4*k);               /* binomial supposedly returns a seq_t */
                bin = mul_small(bin, (unsigned)(k + 1));                 /* multiply by (k+1) */
                sum += bin;
            }
            cache[i] = sum;
        }
    }

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    if (n >= cache_size)            /* sanity guard – should never happen */
        return (seq_t)0;
    return cache[n];
}
