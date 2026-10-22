/*
 * A347027  a(1) = 1; a(n) = a(n-1) + 2 * a(floor(n/2)).
 * https://oeis.org/A347027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n) = a(n-1) + 2*a(floor(n/2))
 * Range:     u8 n <= 12, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization.  cache[i] holds a(i). */
static seq_t *cache = NULL;
static idx_t cache_size = 0;   /* largest index computed so far */

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size)
        return;                     /* already computed */

    /* Allocate (n+1) elements: indices 0..n (0 unused) */
    size_t new_cap = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                     /* out of memory */

    cache = tmp;

    /* If this is the first allocation, initialise a(1) */
    if (cache_size == 0) {
        cache[0] = (seq_t)0;         /* unused */
        cache[1] = (seq_t)1;         /* a(1) = 1 */
        cache_size = 1;
    }

    /* Compute all missing entries up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t val = cache[i - 1] + (seq_t)2 * cache[i / 2];
        cache[i] = val;
    }
    cache_size = n;
}

/* External function implementing the sequence */
seq_t a(idx_t n)
{
    if (n < 1)           /* Undefined for offset 0; return 0 for safety */
        return (seq_t)0;
    ensure_cache(n);
    return cache[n];
}
