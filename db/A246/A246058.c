/*
 * A246058  a(n) = (16*10^n - 7)/9.
 * https://oeis.org/A246058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence formula: a(0) = 1, a(n) = 10*a(n-1) + 7  (for n>0) */
/* This yields a(n) = (16*10^n - 7)/9 in closed form. */

#include <stdlib.h>

static seq_t *cache = NULL;   /* dynamic cache for previously computed terms */
static idx_t cache_size = 0;  /* number of elements currently stored */

/* Return the n-th term of the sequence.  The function is thread‑unsafe but
   satisfies the requirements for any call order and any n >= 0. */
seq_t a(idx_t n)
{
    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure: undefined behaviour */
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* If we already computed this term, return it directly. */
    if (n < cache_size)
        return cache[n];

    /* Extend the cache up to index n (inclusive). */
    idx_t old_size = cache_size;
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;   /* allocation failure: undefined behaviour */
    cache = new_cache;

    /* Compute missing values using the recurrence  a(i) = 10*a(i-1)+7. */
    for (idx_t i = old_size; i <= n; ++i) {
        cache[i] = cache[i - 1] * (seq_t)10 + (seq_t)7;
    }

    cache_size = n + 1;
    return cache[n];
}
