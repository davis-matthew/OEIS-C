/*
 * A400226  a(0) = 1; a(n) = Sum_{k=1..n} binomial(2*n-k,k) * a(n-k).
 * https://oeis.org/A400226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(2*n-k,k) * a(n-k) Initial value:
 *            a(0) = 1 The recurrence is memoised in a static cache; terms are
 *            computed incrementally regardless of call order
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   binomial_mul_div
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial_mul_div.h"

/* Helper that computes b * num1 * num2 / (den1 * den2) safely.
   For built‑in integral seq_t we use a 128‑bit intermediate.
   For arbitrary‑precision seq_t the natural big‑int operators are used. */
#ifdef __cplusplus

#else

#endif

/* The main sequence function. */
seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t  computed_max = 0;   /* largest n already computed */
    static size_t cache_size   = 0;   /* allocated cache size */

    /* initialise cache on first call (a(0)=1) */
    if (!cache) {
        cache_size   = 1;
        cache        = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache) abort();
        cache[0]     = (seq_t)1;
        computed_max = 0;
    }

    /* grow cache if needed */
    if (n + 1 > cache_size) {
        size_t new_size = cache_size;
        while (new_size <= (size_t)n) new_size <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms */
    for (idx_t m = computed_max + 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        /* binomial(2*m-1,1) = 2*m-1 */
        seq_t bin = (seq_t)(2 * m - 1);

        for (idx_t k = 1; k <= m; ++k) {
            /* add term: binomial(2*m-k,k) * a(m-k) */
            sum += bin * cache[m - k];

            if (k == m) break;  /* last iteration done */

            /* compute next binomial using recurrence:
               C(2m-(k+1),k+1) = C(2m-k,k) *
                 (2m-2k)*(2m-2k-1) / ((k+1)*(2m-k))                */
            unsigned long long num1 = 2 * m - 2 * k;
            unsigned long long num2 = 2 * m - 2 * k - 1;
            unsigned long long den1 = k + 1;
            unsigned long long den2 = 2 * m - k;
            bin = binomial_mul_div(bin, num1, num2, den1, den2);
        }

        cache[m] = sum;
    }

    computed_max = n;
    return cache[n];
}
