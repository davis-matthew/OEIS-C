/*
 * A288231  Coefficients of 1/(Sum_{k>=0} [(k+1)*r]*(-x)^k), where r = 4^(1/3)
 *          and [ ] = floor.
 * https://oeis.org/A288231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in 1/(sum_{k>=0}
 *            floor((k+1)*4^(1/3))*(-x)^k)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 32, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence is computed by re‑using the power‑series inversion recurrence:      */
/*   g0 = 1, and for n>0:  g_n = - sum_{k=1..n} s_k * g_{n-k}                     */
/*   where s_k = floor((k+1)*4^(1/3)) * (-1)^k                                      */
/* The floor((k+1)*4^(1/3)) is obtained exactly as floor(cuberoot(4*(k+1)^3)).       */

#include <stdint.h>
#include <stdlib.h>

/* integer cube‑root floor for uint64_t arguments */
static uint64_t icbrt(uint64_t x) {
    uint64_t lo = 0, hi = 1;
    /* find an upper bound */
    while ((__int128)hi * hi * hi <= x) hi <<= 1;
    /* binary search */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        __int128 cube = (__int128)mid * mid * mid;
        if (cube <= x) lo = mid; else hi = mid - 1;
    }
    return lo;
}

/* a loop that returns floor((k+1)*4^(1/3)) for arbitrary k */
static uint64_t coeff_a(uint64_t k) {
    uint64_t kp1 = k + 1;
    __int128 cube = (__int128)kp1 * kp1 * kp1;   /* (k+1)^3 */
    cube *= 4;                                  /* 4*(k+1)^3 */
    return icbrt((uint64_t)cube);
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    /* static cache for computed values */
    static size_t cache_size = 0;          /* number of terms stored (>= 1)   */
    static seq_t *g = NULL;                /* coefficients g[0..cache_size-1] */
    static uint64_t *a_coeff = NULL;       /* a_k values for k=0..cache_size-1 */

    /* initialise cache for n = 0 */
    if (cache_size == 0) {
        cache_size = 1;
        g = (seq_t *)malloc(cache_size * sizeof(seq_t));
        a_coeff = (uint64_t *)malloc(cache_size * sizeof(uint64_t));
        if (!g || !a_coeff) return 0; /* allocation failure, return 0 */
        g[0] = (seq_t)1;                      /* g(0) */
        a_coeff[0] = coeff_a(0);              /* a_0 */
    }

    /* extend cache if needed */
    if ((idx_t)n >= cache_size) {
        size_t new_size = cache_size * 2;
        while ((idx_t)new_size <= n) new_size <<= 1;
        g = (seq_t *)realloc(g, new_size * sizeof(seq_t));
        a_coeff = (uint64_t *)realloc(a_coeff, new_size * sizeof(uint64_t));
        if (!g || !a_coeff) return 0; /* allocation failure */
        for (size_t i = cache_size; i < new_size; ++i) {
            a_coeff[i] = 0; /* will be filled later */
        }
        cache_size = new_size;
    }

    /* compute new terms up to n */
    for (size_t i = (size_t)a_coeff[0] != 0 ? 1 : 1; i <= (size_t)n; ++i) {
        /* ensure we have a_coeff[i] */
        if (a_coeff[i] == 0) a_coeff[i] = coeff_a((uint64_t)i);
        seq_t pos = 0, neg = 0; /* accumulate positive and negative parts separately */
        for (size_t k = 1; k <= i; ++k) {
            seq_t prod = g[i - k] * (seq_t)a_coeff[k];
            if (k & 1) {          /* odd k => negative sign */
                neg += prod;
            } else {              /* even k => positive sign */
                pos += prod;
            }
        }
        /* g[i] = neg - pos (neg >= pos for this sequence) */
        g[i] = neg - pos;
    }

    return g[(size_t)n];
}
