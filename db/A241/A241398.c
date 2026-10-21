/*
 * A241398  Number of 2Xn 0..3 arrays with no element equal to zero plus the
 *          sum of elements to its left or one plus the sum of the elements
 *          above it or zero plus the sum of the elements diagonally to its
 *          northwest or one plus the sum of the elements antidiagonally to
 *          its northeast, modulo 4.
 * https://oeis.org/A241398
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence (empirical, from OEIS A241398):
   a(n) = 2*a(n-2) + 8*a(n-3) - a(n-4) -10*a(n-5) -18*a(n-6) + 8*a(n-7)
          +20*a(n-8) +20*a(n-9) -24*a(n-10) -22*a(n-11) -a(n-12) +32*a(n-13)
          +8*a(n-14) -3*a(n-15) -12*a(n-16) +4*a(n-17) -5*a(n-18) -4*a(n-20)
          -3*a(n-21) +2*a(n-22)   for n > 23
   Base terms a(1)..a(23) are stored explicitly. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;  /* array of a(1)..a(N) stored at cache[0]..cache[N-1] */
    static idx_t cached = 0;     /* highest index that has been computed (>=1) */

    /* First 23 terms */
    static const seq_t init23[23] = {
        3ULL, 5ULL, 9ULL, 23ULL, 44ULL, 93ULL, 204ULL, 368ULL, 761ULL, 1583ULL,
        2951ULL, 6056ULL, 12244ULL, 23588ULL, 47945ULL, 95715ULL,
        187731ULL, 378531ULL, 752679ULL, 1489014ULL, 2986657ULL, 5934705ULL,
        11787915ULL
    };

    /* Ensure the cache holds at least 23 terms */
    if (cached == 0) {
        cached = 23;
        cache = (seq_t *)malloc(cached * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* allocation failed */
        for (idx_t i = 0; i < cached; ++i)
            cache[i] = init23[i];
    }

    /* If n is within already computed range, simply return it */
    if (n <= cached)
        return cache[n-1];

    /* Extend cache to hold terms up to n */
    if (n > cached) {
        idx_t newSize = n;
        seq_t *tmp = (seq_t *)realloc(cache, newSize * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            cached = 0;
            return (seq_t)0;
        }
        cache = tmp;
        cached = newSize;
    }

    /* Coefficients and corresponding offsets for the recurrence */
    static const int offsets[20] = { 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,20,21,22 };
    static const int coeffs [20] = { 2, 8,-1,-10,-18, 8,20,20,-24,-22,-1,32, 8,-3,-12, 4,-5,-4,-3, 2 };

    /* Compute terms from the first missing index up to n */
    for (idx_t idx = 23; idx < n; ++idx) {   /* idx is 0‑based for cache, i.e. a(idx+1) */
        seq_t pos = 0, neg = 0;              /* separate positive and negative accumulation */
        for (int k = 0; k < 20; ++k) {
            idx_t offset = offsets[k];
            seq_t val = cache[idx - offset];   /* cache is 0‑based, a(i) at cache[i-1] */
            int c = coeffs[k];
            if (c >= 0)
                pos += (seq_t)c * val;
            else
                neg += (seq_t)(-c) * val;
        }
        cache[idx] = pos - neg;      /* guaranteed non‑negative */
    }

    return cache[n-1];
}
