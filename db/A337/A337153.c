/*
 * A337153  a(n) = 3^n * (n!)^2 * Sum_{k=0..n} 1 / ((-3)^k * (k!)^2).
 * https://oeis.org/A337153
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n^2*a(n-1) + (-1)^n; with a(0) = 1 (OEIS A337153)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache      = NULL;   /* array of computed terms */
static size_t cache_cap  = 0;      /* current capacity of the array */
static size_t cache_len  = 0;      /* number of terms already computed (next index to fill) */

/* Ensure the cache can hold at least (new_cap) elements */
static void ensure_cap(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    size_t cap = cache_cap == 0 ? 1 : cache_cap;
    while (cap < new_cap)
        cap <<= 1;                     /* double until large enough */
    seq_t *new_cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
    cache      = new_cache;
    cache_cap  = cap;
}

/* Computes the n‑th term of the sequence.  Memoization allows calls in any order. */
seq_t a(idx_t n)
{
    /* If no values have been computed yet, start with a(0)=1. */
    if (cache_len == 0) {
        ensure_cap(1);
        cache[0] = 1;
        cache_len = 1;
    }

    /* Compute missing terms up to n. */
    while (cache_len <= (size_t)n) {
        idx_t i = cache_len;                 /* current index to compute */
        seq_t term = (seq_t)3 * (seq_t)i * (seq_t)i * cache[i-1];
        if ((i & 1ULL) == 0)                 /* add +1 for even i */
            term = term + 1;
        else                                 /* subtract 1 for odd i */
            term = term - 1;
        ensure_cap(cache_len + 1);
        cache[cache_len] = term;
        cache_len++;
    }

    return cache[n];
}
