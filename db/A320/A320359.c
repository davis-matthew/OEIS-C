/*
 * A320359  Number of nX3 0..1 arrays with every element unequal to 2, 3, 4 or
 *          6 horizontally, vertically or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A320359
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 4*a(n-2) + 9*a(n-3) - 19*a(n-4) + 12*a(n-5) -
 *            24*a(n-6) + 5*a(n-7) - 9*a(n-8) + 28*a(n-9) + 20*a(n-10) -
 *            13*a(n-11) - 31*a(n-12) - 8*a(n-13) + 14*a(n-14) + 19*a(n-15) +
 *            4*a(n-16) - a(n-17) - 2*a(n-18) for n > 20 Base values for n = 1
 *            … 20 are listed below. The function works for...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 32, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#if defined(__cplusplus)
#include <type_traits>
#endif

/* Base values for n = 1 … 20 */
static const seq_t init_vals[20] = {
    0ULL,            /* a(1)   */
    1ULL,            /* a(2)   */
    4ULL,            /* a(3)   */
    13ULL,           /* a(4)   */
    60ULL,           /* a(5)   */
    258ULL,          /* a(6)   */
    1111ULL,         /* a(7)   */
    4836ULL,         /* a(8)   */
    21007ULL,        /* a(9)   */
    91214ULL,        /* a(10)  */
    396170ULL,       /* a(11)  */
    1720608ULL,      /* a(12)  */
    7472694ULL,      /* a(13)  */
    32454615ULL,     /* a(14)  */
    140953263ULL,    /* a(15)  */
    612172021ULL,    /* a(16)  */
    2658715999ULL,   /* a(17)  */
    11547033504ULL,  /* a(18)  */
    50149763813ULL,  /* a(19)  */
    217804756979ULL  /* a(20)  */
};

/* Recurrence coefficients (sign included) for a(n) with n>20 */
static const int8_t coeff[18] = {
    5,  -4,   9,  -19,   12,  -24,    5,   -9,
    28,  20, -13, -31,   -8,   14,    19,    4,
   -1,  -2
};

/* Multiply a seq_t value by a small unsigned integer (≤31) safely */
#if defined(__cplusplus)
static inline seq_t mul_small(seq_t v, unsigned m) {
    if constexpr (std::is_arithmetic<seq_t>::value) {
        return (seq_t)((unsigned __int128)v * m);
    } else {
        seq_t res = 0;
        for (unsigned i = 0; i < m; ++i) res += v;
        return res;
    }
}
#else
static inline seq_t mul_small(seq_t v, unsigned m) {
    return (seq_t)((unsigned __int128)v * (unsigned)m);
}
#endif

/* Memoisation storage */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored (≥0) */
static size_t cache_cap = 0;   /* allocated capacity */

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for <1, but harmless */

    /* Return base value if within pre‑computed range */
    if (n <= 20) return init_vals[n - 1];

    /* Ensure cache capacity and initialise with base values */
    if (cache_len == 0) {
        cache_cap = n > 20 ? n : 20;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;        /* out of memory */
        memcpy(cache, init_vals, sizeof(init_vals));
        cache_len = 20;
    } else if (n > cache_cap) {     /* grow if needed */
        cache_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return 0;
        cache = tmp;
    }

    /* Compute missing terms up to n */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i is zero‑based index of term to compute (a(i+1)) */
        seq_t sum = 0;
        for (int k = 0; k < 18; ++k) {
            seq_t prod = mul_small(cache[i - (k + 1)], (unsigned) (coeff[k] < 0 ? -coeff[k] : coeff[k]));
            if (coeff[k] < 0)
                sum -= prod;
            else
                sum += prod;
        }
        cache[i] = sum;
    }
    cache_len = n;                   /* now holds terms up to n */

    return cache[n - 1];
}
