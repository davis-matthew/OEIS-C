/*
 * A373655  a(n) = Sum_{k=0..floor(n/3)} binomial(3*n-8*k-1,k).
 * https://oeis.org/A373655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A373655
   a(n) = Sum_{k=0..⌊n/3⌋} binomial(3*n-8*k-1, k)
   Recurrence for n > 9:
   a(n) = a(n-1) + 3*a(n-3) - 3*a(n-6) + a(n-9)   (initial values a(0)..a(9) below) */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= (size_t)n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initial terms for n = 0..9 */
    static const seq_t init[10] = {
        (seq_t)1, (seq_t)1, (seq_t)1, (seq_t)1,
        (seq_t)4, (seq_t)7, (seq_t)10, (seq_t)19,
        (seq_t)37, (seq_t)64
    };

    if (n < 10) {
        return init[(size_t)n];
    }

    /* Ensure cache contains values up to n */
    if (!cache) {
        /* Allocate initial cache and copy known terms */
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < 10; ++i) cache[i] = init[i];
    }
    ensure_cache(n);

    /* Compute missing terms using recurrence */
    size_t i;
    for (i = 10; i <= (size_t)n; ++i) {
        seq_t v = cache[i-1];
        v += 3 * cache[i-3];
        v -= 3 * cache[i-6];
        v += cache[i-9];
        cache[i] = v;
    }

    return cache[(size_t)n];
}
