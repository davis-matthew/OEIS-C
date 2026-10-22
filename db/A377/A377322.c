/*
 * A377322  Number of cells that are a distance of n away in an order-5
 *          hyperbolic square tiling.
 * https://oeis.org/A377322
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + 2*x + 4*x^2 + 2*x^3 + x^4) / (1 - 2*x - 2*x^3 +
 *            x^4); Recurrence: a(n) = 2*a(n-1) + 2*a(n-3) - a(n-4) for n >=
 *            5, with initial terms: a(0)=1, a(1)=4, a(2)=12, a(3)=28, a(4)=64
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 25, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed terms */
static idx_t seq_cache_size = 0;  /* number of terms stored in the cache */

/* Computes the n-th term of A377322 using a linear recurrence with caching. */
seq_t a(idx_t n)
{
    /* Initialize the cache with the first five terms if necessary. */
    if (seq_cache == NULL) {
        seq_cache_size = 5;
        seq_cache = (seq_t *)malloc(seq_cache_size * sizeof(seq_t));
        if (seq_cache == NULL) return 0;            /* allocation failure */
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)4;
        seq_cache[2] = (seq_t)12;
        seq_cache[3] = (seq_t)28;
        seq_cache[4] = (seq_t)64;
    }

    /* If the requested index is already cached, return it directly. */
    if (n < seq_cache_size) return seq_cache[n];

    /* Grow the cache to accommodate the requested index. */
    idx_t old_size = seq_cache_size;
    if (n + 1 > old_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
        if (tmp == NULL) return 0;           /* allocation failure */
        seq_cache = tmp;
        seq_cache_size = new_size;
    }

    /* Compute missing terms using the recurrence relation. */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t val = seq_cache[i - 1] + seq_cache[i - 1];       /* 2*a(i-1) */
        val += seq_cache[i - 3] + seq_cache[i - 3];           /* +2*a(i-3) */
        val -= seq_cache[i - 4];                             /* -a(i-4) */
        seq_cache[i] = val;
    }

    return seq_cache[n];
}
