/*
 * A391892  a(n) = Sum_{k=0..n} (k+1) * 2^(n-k) * binomial(2*k+1,2*n-2*k+1).
 * https://oeis.org/A391892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A391892
 * a(n) = Sum_{k=0..n} (k+1) * 2^(n-k) * binomial(2*k+1,2*n-2*k+1)
 * Recurrence (valid for n ≥ 8):
 * a(n) = 4*a(n-1) + 2*a(n-2) - 4*a(n-3) - 33*a(n-4)
 *        - 8*a(n-5) + 8*a(n-6) + 32*a(n-7) - 16*a(n-8)
 * Initial values:
 * a(0)=1, a(1)=6, a(2)=19, a(3)=88, a(4)=337,
 * a(5)=1242, a(6)=4623, a(7)=16652
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static idx_t lastComputed = 0;

    /* Explicit initial eight terms (cast to seq_t for all build types) */
    static const seq_t init[8] = {
        (seq_t)1, (seq_t)6, (seq_t)19, (seq_t)88,
        (seq_t)337, (seq_t)1242, (seq_t)4623, (seq_t)16652
    };

    /* Ensure the cache is large enough to hold indices 0..n and the first eight terms */
    size_t need = n + 1;
    if (need < 8) need = 8;
    if (need > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) return (seq_t)0;        /* allocation failure: undefined value */
        cache = tmp;
        /* Initialize newly allocated slots to zero */
        for (size_t i = cap; i < need; ++i) cache[i] = (seq_t)0;
        cap = need;
    }

    /* Initialize the first eight entries on the first run */
    if (lastComputed < 7) {
        for (idx_t i = 0; i < 8; ++i) cache[i] = init[i];
        lastComputed = 7;
    }

    /* If the requested n is within precomputed range, return it */
    if (n <= 7) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = lastComputed + 1; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1] + (seq_t)2 * cache[i-2]
                 - (seq_t)4 * cache[i-3] - (seq_t)33 * cache[i-4]
                 - (seq_t)8 * cache[i-5] + (seq_t)8 * cache[i-6]
                 + (seq_t)32 * cache[i-7] - (seq_t)16 * cache[i-8];
    }
    lastComputed = n;
    return cache[n];
}
