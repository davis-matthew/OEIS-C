/*
 * A295518  a(n) = e^2 * Sum_{k=0..n-1} Gamma(k + 1, 2).
 * https://oeis.org/A295518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-4)*a(n-3)+(3-3*n)*a(n-2)+(2+n)*a(n-1), a(0)=0,
 *            a(1)=1, a(2)=4
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t  cap   = 0;   /* allocated capacity of cache */
    static idx_t   next  = 0;   /* first index that has not been computed yet */

    /* Ensure cache can hold index n */
    if ((idx_t)cap <= n) {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cap ? cap : 10;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;          /* allocation failed */
        cache = new_cache;
        cap   = new_cap;
    }

    /* Initialise first three values once */
    if (next == 0) {
        cache[0] = (seq_t)0;
        if (cap > 1) cache[1] = (seq_t)1;
        if (cap > 2) cache[2] = (seq_t)4;
        next = 3;
    }

    /* Base case for n < 3 */
    if (n < 3) return cache[(size_t)n];

    /* Compute missing terms up to n */
    for (idx_t i = next; i <= n; ++i) {
        seq_t t1 = ((seq_t)2 * (seq_t)i - (seq_t)4) * cache[i - 3];
        seq_t t2 = ((seq_t)2 + (seq_t)i) * cache[i - 1];
        seq_t t3 = ((seq_t)3 * (seq_t)i - (seq_t)3) * cache[i - 2];
        cache[i] = t1 + t2 - t3;
    }
    next = n + 1;
    return cache[(size_t)n];
}
