/*
 * A263907  Number of (2n+2) X (2+2) 0..1 arrays with each row and column
 *          modulo 3 equal to 1, read as a binary number with top and left
 *          being the most significant bits.
 * https://oeis.org/A263907
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (3 - 4*7^(1+n) + 25^(1+n)) / 27 Equivalent recurrence:
 *            a(n) = 33*a(n-1) - 207*a(n-2) + 175*a(n-3) Initial values:
 *            a(1)=16, a(2)=528, a(3)=14112
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;      /* cached values, indices 0..N (0 unused) */
static size_t cache_cap = 0;     /* current allocated size (>= 4) */
static idx_t last_computed = 0;  /* largest n already computed */

/* Ensure cache has space for indices 0..n.
   Guarantees at least 4 elements so that a(3) is accessible. */
static void ensure_cache(idx_t n)
{
    size_t new_cap = (size_t)n + 1;
    if (new_cap < 4) new_cap = 4;
    if (new_cap <= cache_cap) return;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failed – abort the program */
        abort();
    }
    /* initialize newly allocated elements to zero (safe for unsigned types) */
    for (size_t i = cache_cap; i < new_cap; ++i) new_cache[i] = 0;
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* out of range; undefined behaviour otherwise */

    ensure_cache(n);

    /* initialise base values only if not already set */
    if (cache[1] == 0) cache[1] = (seq_t)16;
    if (cache[2] == 0) cache[2] = (seq_t)528;
    if (cache[3] == 0) cache[3] = (seq_t)14112;

    if (n <= last_computed) return cache[n];

    /* make sure we have at least the first three terms before recurrence */
    if (last_computed < 3) last_computed = 3;

    for (idx_t k = last_computed + 1; k <= n; ++k) {
        /* recurrence: a(k) = 33*a(k-1) - 207*a(k-2) + 175*a(k-3) */
        cache[k] = 33 * cache[k-1] - 207 * cache[k-2] + 175 * cache[k-3];
    }
    last_computed = n;
    return cache[n];
}
