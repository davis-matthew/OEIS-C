/*
 * A289785  p-INVERT of the (5^n), where p(S) = 1 - S - S^2.
 * https://oeis.org/A289785
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 29*a(n-2) with a(0) = 1, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t computed = 0;

/* Grow cache to hold at least n+1 entries */
static void ensure_capacity(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();           /* cannot allocate */
    cache = tmp;
    /* initialise new entries to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* handle the two base cases first */
    if (n == 0) {
        ensure_capacity(0);
        if (computed == 0) { cache[0] = 1; computed = 1; }
        return cache[0];
    }
    if (n == 1) {
        ensure_capacity(1);
        if (computed == 0) { cache[0] = 1; cache[1] = 7; computed = 2; }
        else if (computed == 1) { cache[1] = 7; computed = 2; }
        return cache[1];
    }

    /* n >= 2 */
    ensure_capacity(n);
    if (computed == 0) { cache[0] = 1; cache[1] = 7; computed = 2; }

    if (n < computed)
        return cache[(size_t)n];

    for (idx_t i = computed; i <= n; ++i) {
        seq_t a1 = cache[(size_t)(i - 1)];
        seq_t a2 = cache[(size_t)(i - 2)];
        cache[(size_t)i] = 11 * a1 - 29 * a2;
    }
    computed = n + 1;
    return cache[(size_t)n];
}
