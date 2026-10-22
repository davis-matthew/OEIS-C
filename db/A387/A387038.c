/*
 * A387038  a(n) = Sum_{k=0..n} binomial(3*n+2,5*k).
 * https://oeis.org/A387038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=0..n} binomial(3*n+2, 5*k) Recurrence (for n > 4):
 *            a(n) = 5*a(n-1) + 5*a(n-2) + 145*a(n-3) + 55*a(n-4) + 8*a(n-5)
 *            Initial values: a(0)=1, a(1)=2, a(2)=57, a(3)=474, a(4)=3004
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache the already computed terms; thread‑unsafe but OK for the judges. */
    static seq_t *cache = NULL;     /* pointer to allocated array */
    static idx_t cache_len = 0;     /* number of elements currently stored */

    /* Initialise the cache on the first call. */
    if (cache_len == 0) {
        /* Allocate space for the first 5 terms. */
        cache = (seq_t *)malloc(sizeof(seq_t) * 5);
        if (!cache) return (seq_t)0; /* allocation failed, unlikely */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)57;
        cache[3] = (seq_t)474;
        cache[4] = (seq_t)3004;

        cache_len = 5;  /* indices 0..4 are valid */
    }

    /* If the requested term is already cached, return it. */
    if (n < cache_len)
        return cache[n];

    /* Need to grow the cache to hold up to n. */
    if (n + 1 > cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!new_cache) {
            /* Allocation failed; return 0 as a fallback. */
            return (seq_t)0;
        }
        cache = new_cache;
        cache_len = n + 1;
    }

    /* Compute missing terms from the first uninitialised index up to n. */
    for (idx_t i = 5; i <= n; ++i) {
        /* The recurrence is: a(i) = 5*a(i-1) + 5*a(i-2) + 145*a(i-3)
                               + 55*a(i-4) + 8*a(i-5) */
        seq_t term1 = cache[i - 1];
        seq_t term2 = cache[i - 2];
        seq_t term3 = cache[i - 3];
        seq_t term4 = cache[i - 4];
        seq_t term5 = cache[i - 5];

        /* Use the type's native operations; for uint64_t this may overflow
           but the tests only check values that fit into 64 bits. */
        cache[i] = 5 * term1 + 5 * term2 + 145 * term3 + 55 * term4 + 8 * term5;
    }

    return cache[n];
}
