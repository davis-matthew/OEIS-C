/*
 * A370231  a(1) = 1; for n >= 2, a(n) = a(GCD(n - 1, a(n - 1))) + floor((n -
 *          1)/a(n - 1)) + 1.
 * https://oeis.org/A370231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n >= 2, a(n) = a(GCD(n - 1, a(n - 1))) + floor((n
 *            - 1)/a(n - 1)) + 1 (computed recursively with memoisation)
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"   /* seq_t gcd(seq_t a, seq_t b); */

static seq_t *cache = NULL;  /* cache[0] unused; cache[1] = 1 */
static size_t cache_cap = 0; /* allocated size */

/* Ensure cache can hold indices up to n; grow using realloc */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE); /* abort on OOM */
    cache = new_cache;
    /* initialise new entries to 0 (just in case) */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = (seq_t)0;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;             /* undefined, return 0 */
    ensure_cache(n);
    if (cache[1] == (seq_t)0) cache[1] = (seq_t)1; /* initialise a(1) */

    /* Compute terms from currently known up to n */
    for (size_t i = 2; i <= (size_t)n; ++i) {
        if (cache[i] != (seq_t)0) continue;   /* already computed */

        seq_t prev = cache[i - 1];                     /* a(i-1) */
        idx_t g = (idx_t)gcd((seq_t)(i - 1), prev);    /* GCD(n-1, a(n-1)) */
        seq_t div = (seq_t)( (seq_t)(i - 1) / prev );  /* floor((n-1)/a(n-1)) */
        cache[i] = cache[g] + div + (seq_t)1;           /* recurrence */
    }
    return cache[n];
}
