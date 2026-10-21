/*
 * A256745  Number of (n+2)X(5+2) 0..1 arrays with no 3x3 subblock diagonal
 *          sum 3 and no antidiagonal sum 3 and no row sum 0 and no column sum
 *          0.
 * https://oeis.org/A256745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = recurrence started at a(1) = 5255 a(n) = -2·a(n-1) +
 *            2·a(n-2) +16·a(n-3)+25·a(n-4)-14·a(n-5)-50·a(n-6)
 *            -64·a(n-7)-120·a(n-8)-296·a(n-9)-112·a(n-10)+1088·a(n-11)
 *            +1728·a(n-12)+384·a(n-13)-2048·a(n-14)-2048·a(n-15) for n>18,
 *            with the 18 given starting terms
 * Range:     u8 none, u16 n <= 6, u32 n <= 23, u64 n <= 56, u128 n <= 119,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Dependencies */
#include <stdlib.h>

/* Coefficients of the 15‑term recurrence, in order a(n-1) … a(n-15). */
static const int coeff[15] = {
    -2,   2,  16,  25, -14, -50, -64, -120,
   -296, -112,1088,1728, 384,-2048,-2048
};

/* First 18 terms of the sequence.  They are cast to seq_t when stored in
   the cache. */
static const unsigned long long init_raw[18] = {
    5255ULL, 6352ULL, 9112ULL, 10360ULL, 22080ULL, 47440ULL,
    96576ULL, 160704ULL, 300864ULL, 463632ULL,
    1034880ULL, 2109632ULL, 4284608ULL, 6583568ULL,
    14819840ULL, 24554944ULL, 53916864ULL, 105740048ULL
};

/* Static cache: cache[0] holds a(1), cache[1] holds a(2), etc. */
static seq_t *cache = NULL;       /* pointer to allocated array */
static size_t cache_len = 0;      /* number of terms currently stored */

/* ---------------------------------------------------------------------- */
/*  a(n) : return the nth term (1‑based index)                           */
seq_t a(idx_t n)
{
    if (n == 0) {                 /* a(0) is undefined for this seq. */
        return (seq_t)0;
    }

    /* Convert n to size_t only after ensuring it fits (n <= 10000 by spec). */
    size_t target = (size_t)n;

    /* Allocate and initialise cache once. */
    if (cache == NULL) {
        /* Start with space for the 18 known terms or the requested size,
           whichever is larger. */
        size_t init_size = target > 18 ? target : 18;
        cache = (seq_t*)malloc(init_size * sizeof(seq_t));
        if (!cache) abort();       /* cannot proceed without memory */

        for (size_t i = 0; i < 18; ++i) {
            cache[i] = (seq_t)init_raw[i];
        }
        cache_len = 18;
    }

    /* If we already have the requested term, just return it. */
    if (target <= cache_len) {
        return cache[target - 1];
    }

    /* Grow the cache to at least 'target' terms.  Use realloc to avoid
       unnecessary fragmentation for small n. */
    if (target > cache_len) {
        seq_t *tmp = (seq_t*)realloc(cache, target * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
    }

    /* Compute new terms up to n using the recurrence. */
    for (size_t idx = cache_len; idx < target; ++idx) {
        seq_t sum = 0;                          /* seq_t is unsigned or big int */
        for (int i = 0; i < 15; ++i) {
            size_t prev = idx - 1 - i;          /* previous term a(idx-i) */
            seq_t prod = cache[prev] * (seq_t) (coeff[i] >= 0 ? coeff[i] : -coeff[i]);

            if (coeff[i] >= 0) {
                sum += prod;
            } else {
                sum -= prod;
            }
        }
        cache[idx] = sum;   /* value for a(idx+1) */
    }

    cache_len = target;
    return cache[target - 1];
}
