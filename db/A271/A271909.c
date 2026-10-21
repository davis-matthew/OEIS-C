/*
 * A271909  Numbers k such that k and 3*k+1 have the same number of prime
 *          divisors (including multiplicities).
 * https://oeis.org/A271909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 9359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find k such that bigomega(k) == bigomega(3*k + 1) */
#include <stdlib.h>
#include "../utils/bigomega.h"

static seq_t *cache = NULL;          /* storage for found k values */
static size_t cache_len = 0;         /* number of terms stored */
static size_t cache_cap = 0;         /* current capacity of cache */
static idx_t next_k = 1;             /* next k to test */

/* Ensure the cache can hold 'needed' elements */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
    while (new_cap < needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!cache) abort();          /* out of memory */
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* outside OEIS offset */
    if (cache_len >= n) return cache[n - 1];   /* already computed */

    grow_cache(cache_len + 1);

    idx_t k = next_k;
    while (cache_len < n) {
        seq_t w1 = bigomega(k);
        idx_t mk = 3 * k + 1;
        seq_t w2 = bigomega(mk);
        if (w1 == w2) {
            if (cache_len == cache_cap)
                grow_cache(cache_len + 1);
            cache[cache_len++] = (seq_t)k;
        }
        ++k;
    }
    next_k = k;                                 /* next search starts here */
    return cache[n - 1];
}
