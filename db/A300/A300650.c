/*
 * A300650  Number of orderless same-trees of weight 2n + 1 in which all
 *          outdegrees are odd and all leaves greater than 1.
 * https://oeis.org/A300650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A300650
   Formula: a(0)=1;
           for n>0,
           a(n) = Σ_{odd d>1 , d| (2n+1)} binomial( a(⌊n/d⌋) + d – 1 , d )     */
/* The binomial is computed multiplicatively to avoid factorial blow‑up. */

#include <stdlib.h>
#include <stdbool.h>

/* seq_t and idx_t are provided by the build system; no definitions here. */

/* ------------------------------------------------------------------
   compute binomial(N, k) with N, k possibly large.  N is a seq_t (any
   unsigned integer size) and k fits in an idx_t.  The result also
   fits in seq_t.  The computation uses only + - * / and integer
   division, which is exact for binomial coefficients.
   ------------------------------------------------------------------ */
static seq_t binomial_big(seq_t N, idx_t k)
{
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= k; ++i) {
        seq_t term = N - (seq_t)(k - i);   /* N - k + i */
        res = res * term;
        res = res / i;
    }
    return res;
}

/* ------------------------------------------------------------------
   Main sequence accessor.
   Uses a static cache so calls may come in any order and are lazy‑
   evaluated.  The cache grows geometrically.
   ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* static cache storage */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated size */
    static size_t cache_len = 0;   /* number of elements already stored */
    static bool initialized = false;

    /* first call: create cache with a(0) */
    if (!initialized) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;        /* a(0) = 1 */
        cache_len = 1;
        initialized = true;
    }

    /* ensure enough room for n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute any missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i == 0) {                /* already set */
            cache[i] = (seq_t)1;
            continue;
        }

        /* weight 2*i+1 (always odd) */
        idx_t w = 2 * i + 1;

        /* collect odd divisors of w greater than 1 */
        idx_t divisors[256];
        size_t dcnt = 0;

        for (idx_t d = 3; d * d <= w; d += 2) {
            if (w % d == 0) {
                divisors[dcnt++] = d;
                if (d != w / d) divisors[dcnt++] = w / d;
            }
        }
        divisors[dcnt++] = w;    /* w itself is a divisor >1 and odd */

        seq_t sum = (seq_t)0;

        for (size_t j = 0; j < dcnt; ++j) {
            idx_t d = divisors[j];
            idx_t nd = i / d;        /* floor(i/d) */
            seq_t base = cache[nd] + (seq_t)(d - 1);
            seq_t term = binomial_big(base, d);
            sum = sum + term;
        }

        cache[i] = sum;
    }

    cache_len = n + 1;
    return cache[n];
}
