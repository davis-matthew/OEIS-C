/*
 * A296330  Number of n X 3 0..1 arrays with each 1 horizontally, vertically
 *          or antidiagonally adjacent to 0 or 2 neighboring 1s.
 * https://oeis.org/A296330
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A296330: Number of n×3 0..1 arrays with each 1 horizontally, vertically
 * or antidiagonally adjacent to 0 or 2 neighboring 1s.
 *
 * Empirical recurrence (valid for n ≥ 10):
 *   a(n) = 2*a(n-1) + 4*a(n-2) + 5*a(n-3) + 2*a(n-4)
 *          - 2*a(n-5) - 3*a(n-6) - 2*a(n-7) + a(n-9)
 *
 * Initial values:
 *   a(1)=5, a(2)=19, a(3)=64, a(4)=230, a(5)=816,
 *   a(6)=2895, a(7)=10277, a(8)=36480, a(9)=129486
 *
 * The function memoises already‑computed terms and works in any call order.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Do not report how a(0) or negative indices are handled. */
    if (n == 0) return 0;

    /* static cache */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static idx_t cache_up_to = 0;   /* largest n already computed */

    /* Ensure space for at least n+1 slots (index 0 unused) and 10 for the recurrence */
    size_t needed = (size_t)n + 1;
    if (needed < 10) needed = 10;
    if (needed > cache_capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        /* zero newly allocated slots (undef but harmless for unsigned types) */
        for (size_t i = cache_capacity; i < needed; ++i) tmp[i] = 0;
        cache = tmp;
        cache_capacity = needed;
    }

    /* initialise known terms on first use */
    if (cache_up_to == 0) {
        cache[1] = 5;
        cache[2] = 19;
        cache[3] = 64;
        cache[4] = 230;
        cache[5] = 816;
        cache[6] = 2895;
        cache[7] = 10277;
        cache[8] = 36480;
        cache[9] = 129486;
        cache_up_to = 9;
    }

    /* Return early if already computed */
    if (n <= cache_up_to) return cache[n];

    /* Compute missing terms up to n using the recurrence */
    for (idx_t i = cache_up_to + 1; i <= n; ++i) {
        seq_t pos = cache[i-1] * (seq_t)2
                  + cache[i-2] * (seq_t)4
                  + cache[i-3] * (seq_t)5
                  + cache[i-4] * (seq_t)2
                  + cache[i-9];
        seq_t neg = cache[i-5] * (seq_t)2
                  + cache[i-6] * (seq_t)3
                  + cache[i-7] * (seq_t)2;
        /* Since all final values are non‑negative, pos must be ≥ neg. */
        cache[i] = pos - neg;
    }
    cache_up_to = n;
    return cache[n];
}
