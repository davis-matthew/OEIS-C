/*
 * A245924  Expansion of (1-x - sqrt(1 - 14*x + x^2)) / (6*x*(1 - 14*x +
 *          x^2)).
 * https://oeis.org/A245924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* D-finite recurrence: (n+1)*a(n) +7*(-4*n-1)*a(n-1) +99*(2*n-1)*a(n-2) +7*(-4*n+5)*a(n-3) +(n-2)*a(n-4) = 0.
   Base values: a(0)=1, a(1)=18, a(2)=279, a(3)=4132. */
#include <stdlib.h>
#include <stdint.h>
#ifndef NULL
#define NULL 0
#endif

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* ensure cache capacity */
    if (n >= cache_size) {
        size_t new_sz = (size_t)(n + 1);
        /* cast realloc for C++ compilers */
        cache = (seq_t*)realloc(cache, new_sz * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failed – return 0 */
        /* initialise new entries to 0 */
        for (size_t i = cache_size; i < new_sz; ++i)
            cache[i] = 0;
        cache_size = new_sz;
    }

    /* base cases */
    if (n == 0) { if (cache[0] == 0) cache[0] = 1; return cache[0]; }
    if (n == 1) { if (cache[1] == 0) cache[1] = 18; return cache[1]; }
    if (n == 2) { if (cache[2] == 0) cache[2] = 279; return cache[2]; }
    if (n == 3) { if (cache[3] == 0) cache[3] = 4132; return cache[3]; }

    /* compute missing values up to n */
    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != 0) continue;   /* already computed by another call */

        /* branching based on the size of seq_t */
        if (sizeof(seq_t) == 8) {
            /* 64‑bit arithmetic – use signed __int128 to avoid overflow */
            uint64_t a1 = (uint64_t)cache[i-1];
            uint64_t a2 = (uint64_t)cache[i-2];
            uint64_t a3 = (uint64_t)cache[i-3];
            uint64_t a4 = (uint64_t)cache[i-4];

            __int128 num = 0;
            uint64_t coeff1 = 28*i + 7;
            uint64_t coeff2 = 99*(2*i - 1);
            uint64_t coeff3 = 28*i - 35;
            uint64_t coeff4 = i - 2;

            num += (__int128)coeff1 * a1;          /* +(28*i+7)*a(i-1) */
            num -= (__int128)coeff2 * a2;          /* -99*(2*i-1)*a(i-2) */
            num += (__int128)coeff3 * a3;          /* +(28*i-35)*a(i-3) */
            num -= (__int128)coeff4 * a4;          /* -(i-2)*a(i-4) */

            cache[i] = (seq_t)(num / (i + 1));
        } else {
            /* arbitrary‑precision arithmetic – use seq_t operations */
            seq_t t1 = cache[i-1];
            t1 = (seq_t)(28*i + 7) * t1;          /* (28*i+7)*a(i-1) */

            seq_t t2 = cache[i-2];
            t2 = (seq_t)99 * (2*i - 1) * t2;      /* 99*(2*i-1)*a(i-2) */

            seq_t t3 = cache[i-3];
            t3 = (seq_t)(28*i - 35) * t3;          /* (28*i-35)*a(i-3) */

            seq_t t4 = cache[i-4];
            t4 = (seq_t)(i - 2) * t4;              /* (i-2)*a(i-4) */

            seq_t num = t1;
            num -= t2;         /* t1 - t2  (t1 > t2 for all i >= 4) */
            num += t3;         /* + t3    */
            num -= t4;         /* - t4    */

            cache[i] = num / (seq_t)(i + 1);
        }
    }

    return cache[n];
}
