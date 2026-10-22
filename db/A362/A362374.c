/*
 * A362374  Number of solutions of y^2 + y = x^3 + x where x and y are in
 *          GF(2^n).
 * https://oeis.org/A362374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=a(2)=a(3)=4; a(n)=2*a(n-2)+4*a(n-3) for n≥4 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of values, indices 0..cache_cap (0 unused) */
static idx_t cache_cap = 0;   /* largest index currently stored */

/* Ensure that cache contains values up to index n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_cap)
        return;

    /* Reallocate (or allocate) space for indices 0..n */
    size_t new_cap = n + 1; /* include index 0, unused */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure – abort as we cannot continue safely */
        abort();
    }
    cache = tmp;

    /* Initialise any newly added entries */
    for (idx_t i = cache_cap + 1; i <= n; ++i) {
        if (i <= 3) {
            cache[i] = (seq_t)4;
        } else {
            seq_t v1 = cache[i - 2];
            seq_t v2 = cache[i - 3];
            cache[i] = (seq_t)2 * v1 + (seq_t)4 * v2;
        }
    }

    cache_cap = n;
}

/* Main sequence entry point */
seq_t a(idx_t n)
{
    /* The sequence is defined only for n ≥ 1. */
    if (n == 0)
        return (seq_t)0; /* value for offset not used */

    ensure_cache(n);
    return cache[n];
}
