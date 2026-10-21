/*
 * A282918  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 529", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + a(n-4) - 4*a(n-6) for n ≥ 11 * Initial values
 *            (OEIS offset 0): * a(0)=1, a(1)=0, a(2)=4, a(3)=0, a(4)=28,
 *            a(5)=32, a(6)=28, * a(7)=128, a(8)=124, a(9)=544, a(10)=284 *
 *            (The first 11 terms are explicitly stored; afterwards the
 *            homogeneous * recurrence applies.)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    /* Initialise cache with the first 11 known terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(11 * sizeof(seq_t));
        if (!cache) abort();            /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)0;
        cache[4] = (seq_t)28;
        cache[5] = (seq_t)32;
        cache[6] = (seq_t)28;
        cache[7] = (seq_t)128;
        cache[8] = (seq_t)124;
        cache[9] = (seq_t)544;
        cache[10] = (seq_t)284;
        cache_len = 11;
    }

    /* Return a cached value if it already exists */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Grow the cache to hold all values up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) abort();
    cache = new_cache;

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* For i > 10: homogeneous recurrence */
        seq_t term = 4 * cache[i - 2] + cache[i - 4] - 4 * cache[i - 6];
        cache[i] = term;
    }

    cache_len = new_len;
    return cache[(size_t)n];
}
