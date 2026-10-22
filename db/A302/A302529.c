/*
 * A302529  Number of 3 X n 0..1 arrays with every element equal to 1, 2, 4 or
 *          5 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) -4*a(n-2) +6*a(n-3) -10*a(n-4) -24*a(n-5) *
 *            +37*a(n-6) -22*a(n-7) +72*a(n-8) +18*a(n-9) -108*a(n-10) *
 *            +24*a(n-11) -24*a(n-12) +24*a(n-13) +8*a(n-14) -8*a(n-15) * for
 *            n > 17, with the first 17 terms below (1‑based indexing): *
 *            a(1)=0, a(2)=10, a(3)=28, a(4)=42, a(5)=100, a(6)=290,...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 24, u64 n <= 48, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute a single term using the recurrence.  The function uses two
   accumulator variables: 'pos' for the sum of positive coefficients and
   'neg' for the sum of absolute values of negative coefficients.  A
   signed __int128 is used for the intermediate sum when seq_t is 64‑bit
   or smaller, otherwise the accumulators have the same type as seq_t. */
static inline seq_t compute_term(idx_t i, seq_t const *cache)
{
    /* Coefficients for offsets 1..15 */
    static const int8_t coeff[15] = {
        4,   /* a(n-1)  */
       -4,   /* a(n-2)  */
        6,   /* a(n-3)  */
       -10,  /* a(n-4)  */
       -24,  /* a(n-5)  */
       37,   /* a(n-6)  */
       -22,  /* a(n-7)  */
        72,   /* a(n-8)  */
        18,   /* a(n-9)  */
       -108, /* a(n-10) */
        24,   /* a(n-11) */
       -24,  /* a(n-12) */
        24,   /* a(n-13) */
        8,    /* a(n-14) */
       -8     /* a(n-15) */
    };

    if (sizeof(seq_t) <= 8) {
        __int128 pos = 0, neg = 0;
        /* Offsets 1..15 are valid because i >= 18 in all callers. */
        pos += (__int128)cache[i-1] * (coeff[0] > 0 ? coeff[0] : 0);
        neg += (__int128)cache[i-1] * (coeff[0] < 0 ? -coeff[0] : 0);
        pos += (__int128)cache[i-2] * (coeff[1] > 0 ? coeff[1] : 0);
        neg += (__int128)cache[i-2] * (coeff[1] < 0 ? -coeff[1] : 0);
        pos += (__int128)cache[i-3] * (coeff[2] > 0 ? coeff[2] : 0);
        neg += (__int128)cache[i-3] * (coeff[2] < 0 ? -coeff[2] : 0);
        pos += (__int128)cache[i-4] * (coeff[3] > 0 ? coeff[3] : 0);
        neg += (__int128)cache[i-4] * (coeff[3] < 0 ? -coeff[3] : 0);
        pos += (__int128)cache[i-5] * (coeff[4] > 0 ? coeff[4] : 0);
        neg += (__int128)cache[i-5] * (coeff[4] < 0 ? -coeff[4] : 0);
        pos += (__int128)cache[i-6] * (coeff[5] > 0 ? coeff[5] : 0);
        neg += (__int128)cache[i-6] * (coeff[5] < 0 ? -coeff[5] : 0);
        pos += (__int128)cache[i-7] * (coeff[6] > 0 ? coeff[6] : 0);
        neg += (__int128)cache[i-7] * (coeff[6] < 0 ? -coeff[6] : 0);
        pos += (__int128)cache[i-8] * (coeff[7] > 0 ? coeff[7] : 0);
        neg += (__int128)cache[i-8] * (coeff[7] < 0 ? -coeff[7] : 0);
        pos += (__int128)cache[i-9] * (coeff[8] > 0 ? coeff[8] : 0);
        neg += (__int128)cache[i-9] * (coeff[8] < 0 ? -coeff[8] : 0);
        pos += (__int128)cache[i-10] * (coeff[9] > 0 ? coeff[9] : 0);
        neg += (__int128)cache[i-10] * (coeff[9] < 0 ? -coeff[9] : 0);
        pos += (__int128)cache[i-11] * (coeff[10] > 0 ? coeff[10] : 0);
        neg += (__int128)cache[i-11] * (coeff[10] < 0 ? -coeff[10] : 0);
        pos += (__int128)cache[i-12] * (coeff[11] > 0 ? coeff[11] : 0);
        neg += (__int128)cache[i-12] * (coeff[11] < 0 ? -coeff[11] : 0);
        pos += (__int128)cache[i-13] * (coeff[12] > 0 ? coeff[12] : 0);
        neg += (__int128)cache[i-13] * (coeff[12] < 0 ? -coeff[12] : 0);
        pos += (__int128)cache[i-14] * (coeff[13] > 0 ? coeff[13] : 0);
        neg += (__int128)cache[i-14] * (coeff[13] < 0 ? -coeff[13] : 0);
        pos += (__int128)cache[i-15] * (coeff[14] > 0 ? coeff[14] : 0);
        neg += (__int128)cache[i-15] * (coeff[14] < 0 ? -coeff[14] : 0);

        return (seq_t)(pos - neg);
    } else {
        /* seq_t is an arbitrary‑precision type.  All operations are
           performed with that type. */
        seq_t pos = 0;
        seq_t neg = 0;
        for (int k = 0; k < 15; ++k) {
            seq_t term = cache[i - 1 - k];
            int8_t c = coeff[k];
            if (c > 0)
                pos += term * (seq_t)c;
            else
                neg += term * (seq_t)(-c);
        }
        return pos - neg;
    }
}

seq_t a(idx_t n)
{
    /* Initial values for n = 1 … 17 (1‑based), index 0 unused. */
    static const seq_t init[18] = {
        0ULL,          /* dummy index 0 */
        0ULL, 10ULL, 28ULL, 42ULL, 100ULL, 290ULL,
        730ULL, 1700ULL, 4246ULL, 10758ULL, 26632ULL,
        65846ULL, 164130ULL, 408716ULL, 1015998ULL, 2527374ULL, 6288152ULL
    };

    if (n < 1) return 0;          /* behaviour below offset not needed */

    if (n < 18) return init[(size_t)n];

    /* Cached terms – 1‑based indexing (cache[0] unused). */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t max_cached = 0;

    if (!cache) {                /* first call – allocate and seed */
        cache_cap = 18;          /* at least 17 elements + dummy 0 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;    /* allocation failure fallback */
        for (idx_t i = 1; i <= 17; ++i)
            cache[i] = init[i];
        max_cached = 17;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap * 2;
        while (new_cap <= (size_t)n + 1)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i)
        cache[i] = compute_term(i, cache);

    max_cached = n;

    return cache[(size_t)n];
}
