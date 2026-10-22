/*
 * A315318  Coordination sequence Gal.6.622.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) - a(n-6) + a(n-7) + a(n-9) -
 *            a(n-10) for n>10 Initial values: a(0)=1, a(1)=6, a(2)=10,
 *            a(3)=16, a(4)=21, a(5)=26, a(6)=31, a(7)=36, a(8)=41, a(9)=46,
 *            a(10)=52
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;           /* cached values */
    static size_t cap = 0;                /* capacity in slots */
    static idx_t max_computed = (idx_t)-1;/* largest n computed so far */

    /* Initialize cache with the first 11 terms (0..10). */
    if (!cache) {
        cap = 11; /* indices 0..10 inclusive */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) abort(); /* allocation failed */

        cache[0] = (seq_t)1;
        cache[1] = (seq_t)6;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)16;
        cache[4] = (seq_t)21;
        cache[5] = (seq_t)26;
        cache[6] = (seq_t)31;
        cache[7] = (seq_t)36;
        cache[8] = (seq_t)41;
        cache[9] = (seq_t)46;
        cache[10] = (seq_t)52;

        max_computed = 10;
    }

    /* Return cached value if already computed. */
    if ((size_t)n <= (size_t)max_computed) {
        return cache[(size_t)n];
    }

    /* Grow cache if needed. */
    if ((size_t)n >= cap) {
        size_t newcap = (size_t)n * 2 + 10; /* buffer growth */
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort(); /* reallocation failed */
        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms using the recurrence. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* recurrence holds for i > 10; i > 10 guaranteed here */
        seq_t sum_pos = cache[(size_t)(i-1)] + cache[(size_t)(i-3)]
                      + cache[(size_t)(i-7)] + cache[(size_t)(i-9)];
        seq_t sum_neg = cache[(size_t)(i-4)] + cache[(size_t)(i-6)]
                      + cache[(size_t)(i-10)];
        cache[(size_t)i] = sum_pos - sum_neg; /* sum_pos >= sum_neg for all n */
    }

    max_computed = n;
    return cache[(size_t)n];
}
