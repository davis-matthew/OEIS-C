/*
 * A387744  a(n) = (1/5) * Sum_{k=0..n-1} binomial(5*n,5*k+2).
 * https://oeis.org/A387744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/5) * Sum_{k=0..n-1} binomial(5*n,5*k+2) Recurrence:
 *            a(0)=0, a(1)=2, a(2)=33, a(n) = 21*a(n-1) + 353*a(n-2) -
 *            32*a(n-3) for n>2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* A memoised cache of previously computed values. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Grow the cache to hold n+1 elements. */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) { /* Allocation failed: abort (behaviour unspecified) */
            return (seq_t)0;
        }
        cache = tmp;

        /* Compute new terms, starting from the current cache size. */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)0;
            } else if (i == 1) {
                cache[1] = (seq_t)2;
            } else if (i == 2) {
                cache[2] = (seq_t)33;
            } else {
                /* Recurrence: a(n) = 21*a(n-1) + 353*a(n-2) - 32*a(n-3) */
                cache[i] = (seq_t)(21 * cache[i - 1] + 353 * cache[i - 2] - 32 * cache[i - 3]);
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
