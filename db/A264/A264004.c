/*
 * A264004  Number of (1+1) X (n+1) arrays of permutations of 0..n*2+1 with
 *          each element having index change (+-,+-) 0,0 1,2 or 1,0.
 * https://oeis.org/A264004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + 12*a(n-3) - 12*a(n-5) * + 4*a(n-6)
 *            - 4*a(n-7) + a(n-8) * initial values: a(1)=4, a(2)=18, a(3)=81,
 *            a(4)=288, * a(5)=1024, a(6)=3872, a(7)=14641, a(8)=54450 * * The
 *            implementation uses memoised recurrence computation. * It stores
 *            previously computed values in a static...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
 
seq_t a(idx_t n)
{
    /* static memoization cache */
    static seq_t *cache = NULL;
    static idx_t cap = 0;                      /* largest n with cache filled */
    static const seq_t init[9] = {
        0,   /* index 0 unused */
        4, 18, 81, 288, 1024, 3872, 14641, 54450
    };

    /* Return known values directly for n <= 8 */
    if (n <= 8) return init[n];

    /* Initialise cache to hold indices 0..8 on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc((9 + 1) * sizeof(seq_t));
        if (!cache) return 0;  /* out‑of‑memory; unlikely in tests */
        cache[0] = 0;
        for (idx_t i = 1; i <= 8; ++i) cache[i] = init[i];
        cap = 8;
    }

    /* Extend cache to cover requested n */
    if (n > cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return 0;  /* out‑of‑memory */

        for (idx_t i = cap + 1; i <= n; ++i) {
            /* compute positive and negative parts separately */
            seq_t pos = cache[i-1] * (seq_t)4;
            pos += cache[i-3] * (seq_t)12;
            pos += cache[i-6] * (seq_t)4;
            pos += cache[i-8];

            seq_t neg = cache[i-2] * (seq_t)4;
            neg += cache[i-5] * (seq_t)12;
            neg += cache[i-7] * (seq_t)4;

            cache[i] = pos - neg;  /* pos >= neg, safe for unsigned */
        }
        cap = n;
    }

    return cache[n];
}
