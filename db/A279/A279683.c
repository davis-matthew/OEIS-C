/*
 * A279683  Number of move operations required to sort all permutations of [n]
 *          by MTF sort.
 * https://oeis.org/A279683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)*n + (n-1)! * (2^(n-1)-1), a(0) = 0 (OEIS A279683)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 29,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "../utils/seq_alloc.h"   /* provides seq_alloc(size_t) */

static seq_t *cache = NULL;      /* cache of computed a(n) values */
static size_t  cache_cap = 0;    /* allocated capacity of cache array   */
static idx_t   cache_len = 0;    /* next free index in cache            */
static seq_t   fact      = 1;    /* current factorial (cache_len-1)!    */
static seq_t   pow2      = 1;    /* current power of two 2^(cache_len-1) */

/* Ensure the cache is large enough to contain indices 0..n */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* initial allocation */
        cache_cap = 2;
        cache = seq_alloc(cache_cap);
        cache[0]   = 0;          /* a(0) = 0 */
        cache_len  = 1;          /* next free index = 1 */
        fact       = 1;          /* 0!  = 1 */
        pow2       = 1;          /* 2^0 = 1 */
    }

    /* Grow array if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 2;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;

        seq_t *new_cache = seq_alloc(new_cap);
        /* copy existing entries – no need to free old cache */
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));

        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Return A279683(n) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    ensure_cache(n);

    if (cache_len > n)             /* value already computed */
        return cache[n];

    /* Compute values for k = cache_len .. n (inclusive) */
    for (idx_t k = cache_len; k <= n; ++k) {
        seq_t term = fact * (pow2 - 1);          /* (k-1)! * (2^(k-1)-1) */
        seq_t ak   = cache[k - 1] * (seq_t)k + term;
        cache[k]   = ak;

        /* Update factorial and power of two for next index */
        fact *= (seq_t)k;
        pow2 <<= 1;

        ++cache_len;                            /* new next free index */
    }

    return cache[n];
}
