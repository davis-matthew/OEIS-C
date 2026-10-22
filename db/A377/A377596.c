/*
 * A377596  a(n) = (a(n-1) + a(n-2))^5 for n>=2 where a(0) = 0, a(1) = 1.
 * https://oeis.org/A377596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (a(n-1) + a(n-2))^5, with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 4, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute x^5 by repeated multiplication; works for any seq_t type */
static inline seq_t pow5(seq_t x)
{
    seq_t y  = x * x;   /* x^2   */
    seq_t y2 = y * y;   /* x^4   */
    seq_t r  = y2 * x;  /* x^5   */
    return r;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Initial trivial cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Static cache of previously computed values */
    static seq_t *cache = NULL;   /* pointer to array of seq_t */
    static idx_t  max_computed = 1;  /* index of largest computed term */
    static size_t cap = 0;          /* current capacity of cache */

    /* On first call, allocate and initialize cache for a(0) and a(1) */
    if (cache == NULL) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
    }

    /* Extend the cache if necessary */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = new_cap;
    }

    /* Compute all terms up to n that are not yet computed */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t sum = cache[i - 1] + cache[i - 2];
        cache[i] = pow5(sum);
    }
    max_computed = n;

    return cache[n];
}
