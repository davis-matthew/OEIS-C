/*
 * A316022  Coordination sequence Gal.6.377.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A316022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A316022: Coordination sequence Gal.6.377.6
   Recurrence (for n > 22):
        a(n) = a(n-1) - a(n-2) + a(n-3) - a(n-4) + a(n-5)
               - a(n-6) + a(n-7) + a(n-8) - a(n-9) + a(n-10)
               - a(n-11) + a(n-12) - a(n-13) + a(n-14) - a(n-15)
   Initial values:
        a(0)=1, a(1)=6, a(2)=9, a(3)=15, a(4)=15, a(5)=21,
        a(6)=30, a(7)=27, a(8)=36, a(9)=39, a(10)=54,
        a(11)=51, a(12)=51, a(13)=63, a(14)=69, a(15)=66,
        a(16)=78, a(17)=72, a(18)=93, a(19)=93, a(20)=81,
        a(21)=105, a(22)=108, a(23)=99 */
#include <stdlib.h>
#if defined(__cplusplus)
#  include <new>
#endif

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;     /* pointer to array of seq_t */
    static size_t cap   = 0;        /* current capacity of cache    */
    static size_t sz    = 0;        /* number of valid entries */

    /* initial values 0..22 (23 numbers) */
    static const seq_t init[23] = {
        1, 6, 9, 15, 15, 21, 30, 27, 36, 39,
        54, 51, 51, 63, 69, 66, 78, 72, 93, 93,
        81, 105, 108
    };

    /* ------------------------------------------------------------------ */
    /* Allocate or grow the cache if necessary.                           */
    /* ------------------------------------------------------------------ */
    if (!cache) {
        size_t init_cap = 32;
        if (init_cap < 23) init_cap = 23;
#if defined(__cplusplus)
        cache = new seq_t[init_cap];
#else
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!cache) abort();
#endif
        cap = init_cap;
    }
    if (cap <= (size_t)n) {
        size_t newcap = cap ? cap * 2 : 32;
        if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        seq_t *tmp;
#if defined(__cplusplus)
        tmp = new seq_t[newcap];
        for (size_t j = 0; j < cap; ++j) tmp[j] = cache[j];
        delete[] cache;
#else
        tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
#endif
        cache = tmp;
        cap   = newcap;
    }

    /* ------------------------------------------------------------------ */
    /* Ensure the first 23 values are present in the cache.               */
    /* ------------------------------------------------------------------ */
    if (sz < 23) {
        for (idx_t i = sz; i < 23; ++i)
            cache[(size_t)i] = init[(size_t)i];
        sz = 23;
    }

    /* Return immediately if the request is within the cached range.     */
    if (n < 23) return cache[(size_t)n];

    /* ------------------------------------------------------------------ */
    /* Compute missing values up to n, using the recurrence.              */
    /* ------------------------------------------------------------------ */
    for (idx_t i = sz; i <= n; ++i) {
        seq_t val = cache[(size_t)(i-1)]
                    - cache[(size_t)(i-2)]
                    + cache[(size_t)(i-3)]
                    - cache[(size_t)(i-4)]
                    + cache[(size_t)(i-5)]
                    - cache[(size_t)(i-6)]
                    + cache[(size_t)(i-7)]
                    + cache[(size_t)(i-8)]
                    - cache[(size_t)(i-9)]
                    + cache[(size_t)(i-10)]
                    - cache[(size_t)(i-11)]
                    + cache[(size_t)(i-12)]
                    - cache[(size_t)(i-13)]
                    + cache[(size_t)(i-14)]
                    - cache[(size_t)(i-15)];
        cache[(size_t)i] = val;
    }
    sz = (size_t)n + 1;
    return cache[(size_t)n];
}
