/*
 * A346272  Sum_{n>=0} a(n) * x^n / (n!)^2 = exp( BesselI(0,2*sqrt(x)) - 1 -
 *          x^3 / 36 ).
 * https://oeis.org/A346272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1, a(1) = 1
   a(n) = n*a(n‑1) + n*(n‑1)²*a(n‑2)/2
          + (1/n) * Σ_{k=4..n} binom(n,k)² * k * a(n‑k)    (n≥2)
   binom(n,k) is computed multiplicatively. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
#ifdef __cplusplus
    /* C++ build: use std::vector. */
    static std::vector<seq_t> cache;
    static idx_t max_computed = 0;                /* highest index stored */

    if (cache.empty()) {                         /* initialise base cases */
        cache.resize(2);
        cache[0] = 1;
        cache[1] = 1;
        max_computed = 1;
    }

    if (n > max_computed) {
        cache.resize(n + 1);                      /* grow as needed */
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t term1 = (seq_t)i * cache[i - 1];
            seq_t term2 = (seq_t)i *
                           (seq_t)(i - 1) *
                           (seq_t)(i - 1) *
                           cache[i - 2];
            term2 /= 2;                           /* division is exact */

            seq_t sum = 0;
            seq_t binom = 1;                      /* binom(i,0)=1 */
            for (idx_t k = 1; k <= i; ++k) {
                binom = binom * (seq_t)(i - k + 1);
                binom = binom / k;
                if (k >= 4) {
                    sum += binom * binom * (seq_t)k * cache[i - k];
                }
            }
            seq_t term3 = sum / i;               /* division is exact */

            cache[i] = term1 + term2 + term3;
        }
        max_computed = n;
    }
    return cache[n];
#else
    /* C build: use raw malloc/realloc.  seq_t is a plain integer type. */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;                   /* allocated size */
    static idx_t max_computed = 0;                /* highest index stored */

    if (cache == NULL) {                          /* initialise base cases */
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache_sz = 2;
        max_computed = 1;
    }

    if (n >= cache_sz) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        cache = tmp;
        cache_sz = n + 1;
    }

    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            seq_t term1 = (seq_t)i * cache[i - 1];
            seq_t term2 = (seq_t)i *
                           (seq_t)(i - 1) *
                           (seq_t)(i - 1) *
                           cache[i - 2];
            term2 /= 2;                           /* division is exact */

            seq_t sum = 0;
            seq_t binom = 1;                      /* binom(i,0)=1 */
            for (idx_t k = 1; k <= i; ++k) {
                binom = binom * (seq_t)(i - k + 1);
                binom = binom / k;
                if (k >= 4) {
                    sum += binom * binom * (seq_t)k * cache[i - k];
                }
            }
            seq_t term3 = sum / i;               /* division is exact */

            cache[i] = term1 + term2 + term3;
        }
        max_computed = n;
    }
    return cache[n];
#endif
}
