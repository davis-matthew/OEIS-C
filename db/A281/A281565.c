/*
 * A281565  Number of 3Xn 0..1 arrays with no element equal to more than two
 *          of its horizontal, diagonal or antidiagonal neighbors, with the
 *          exception of exactly two elements, and with new values introduced
 *          in order 0 sequentially upwards.
 * https://oeis.org/A281565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) -15*a(n-2) +26*a(n-3) -45*a(n-4) +66*a(n-5)
 *            -73*a(n-6) +78*a(n-7) -78*a(n-8) +56*a(n-9) -36*a(n-10)
 *            +24*a(n-11) -8*a(n-12) (n > 20) Initial values (n = 1..20): 0,
 *            1, 68, 621, 2426, 6955, 16656, 36578, 76332, 153813, 303761,
 *            591012, 1135722, 2162149, 4084667, 7664486, 14298612, 265...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 26, u64 n <= 65, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[20] = {
    0ULL, 1ULL, 68ULL, 621ULL, 2426ULL, 6955ULL, 16656ULL,
    36578ULL, 76332ULL, 153813ULL, 303761ULL, 591012ULL,
    1135722ULL, 2162149ULL, 4084667ULL, 7664486ULL,
    14298612ULL, 26540583ULL, 49040949ULL, 90250160ULL
};

seq_t a(idx_t n) {
    /* Handle trivial cases */
    if (n == 0) return 0;                   /* not used, but safe */
    static seq_t *cache = NULL;
    static idx_t cached = 0;                /* number of terms stored (up to n) */

    /* Initialize cache with first 20 terms */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 20);
        for (idx_t i = 0; i < 20; ++i) cache[i] = init_vals[i];
        cached = 20;
    }

    /* Return immediately if we already have the value */
    if (n <= cached) return cache[n - 1];

    /* Grow cache to at least n */
    if (n > cached) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
        if (tmp == NULL) { /* allocation failed */
            /* No special error handling requested; return 0 as fallback */
            return 0;
        }
        cache = tmp;
        /* Compute missing terms up to n using the recurrence */
        for (idx_t k = cached + 1; k <= n; ++k) {
            idx_t idx = k - 1; /* zero‑based index in cache */

            seq_t sumPos = 0;
            seq_t sumNeg = 0;

            sumPos += (seq_t)6 *  cache[idx - 1];   /* a(k-1) */
            sumNeg += (seq_t)15 * cache[idx - 2];   /* a(k-2) */
            sumPos += (seq_t)26 * cache[idx - 3];   /* a(k-3) */
            sumNeg += (seq_t)45 * cache[idx - 4];   /* a(k-4) */
            sumPos += (seq_t)66 * cache[idx - 5];   /* a(k-5) */
            sumNeg += (seq_t)73 * cache[idx - 6];   /* a(k-6) */
            sumPos += (seq_t)78 * cache[idx - 7];   /* a(k-7) */
            sumNeg += (seq_t)78 * cache[idx - 8];   /* a(k-8) */
            sumPos += (seq_t)56 * cache[idx - 9];   /* a(k-9) */
            sumNeg += (seq_t)36 * cache[idx - 10];  /* a(k-10) */
            sumPos += (seq_t)24 * cache[idx - 11];  /* a(k-11) */
            sumNeg += (seq_t)8  * cache[idx - 12];  /* a(k-12) */

            cache[idx] = sumPos - sumNeg;           /* final value */
        }
        cached = n;
    }

    return cache[n - 1];
}
