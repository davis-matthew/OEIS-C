/*
 * A390851  a(n) = Sum_{k=0..n} (-1)^k * binomial(n+k,n-k) * Fibonacci(k+1).
 * https://oeis.org/A390851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * binomial(n+k,n-k) * Fibonacci(k+1)
 *            Recurrence (valid for n ≥ 4): a(0) = 1, a(1) = 0, a(2) = 0, a(3)
 *            = 2 a(n) = 3*a(n-1) - 3*a(n-2) + 3*a(n-3) - a(n-4)
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 31, u64 n <= 59, u128 n <= 117,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t size = 0;

    /* initialise cache with the first four terms */
    if (size == 0) {
        size = 4;                      /* indices 0 .. 3 */
        cache = (seq_t *)malloc(size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* not enough memory: return 0 */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)2;
    }

    /* if the requested index is already computed, return it */
    if (n + 1 <= size)
        return cache[n];

    /* grow the cache to accommodate index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return (seq_t)0;       /* not enough memory: return 0 */
    cache = tmp;

    /* compute missing terms using the recurrence */
    for (idx_t i = size; i < new_size; ++i) {
        /* 3*a(i-1) */
        seq_t t1 = cache[i - 1];
        t1 += cache[i - 1];
        t1 += cache[i - 1];
        /* 3*a(i-2) */
        seq_t t2 = cache[i - 2];
        t2 += cache[i - 2];
        t2 += cache[i - 2];
        /* 3*a(i-3) */
        seq_t t3 = cache[i - 3];
        t3 += cache[i - 3];
        t3 += cache[i - 3];

        seq_t val = t1;
        val -= t2;
        val += t3;
        val -= cache[i - 4];

        cache[i] = val;
    }

    size = new_size;
    return cache[n];
}
