/*
 * A315546  Coordination sequence Gal.5.294.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=6, a(2)=11, a(3)=17, a(4)=23, a(5)=29, a(6)=35,
 *            a(7)=41, a(8)=46, a(9)=52, a(10)=58; for n>10: a(n) = a(n-1) +
 *            a(n-9) - a(n-10)
 * Range:     u8 n <= 44, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache (memoization) */
    static seq_t *cache = NULL;
    static idx_t  cache_len = 0;   /* number of cached values (first index not yet initialized) */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Allocate or grow cache if needed */
    if (n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 32;
        if (new_cap <= (size_t)n) new_cap = (size_t)n + 1;
        /* cast for C++ compilers */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;  /* allocation failure, return zero if possible */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize base terms once */
    if (cache_len == 0) {
        static const seq_t base[11] = {
            1, 6, 11, 17, 23, 29, 35, 41, 46, 52, 58
        };
        for (idx_t i = 0; i < 11; ++i)
            cache[i] = base[i];
        cache_len = 11;
    }

    /* Extend cache up to n if necessary */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            /* recurrence: a(i) = a(i-1) + a(i-9) - a(i-10) */
            seq_t val = cache[i - 1] + cache[i - 9] - cache[i - 10];
            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
