/*
 * A351184  G.f. A(x) satisfies: A(x) = 1 + x + x^2 * A(x/(1 + 3*x)) / (1 +
 *          3*x).
 * https://oeis.org/A351184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 14, i64 n <= 23, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A351184
   a(0) = a(1) = 1
   a(n) = Σ_{k=0}^{n-2} binomial(n-2,k) · (-3)^k · a(n-k-2),  n ≥ 2
   (implemented with memoisation, O(n²) total for up to n ≈ 10⁴) */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache   = NULL;   /* array of computed terms */
static idx_t  cache_len = 0;    /* number of terms currently stored */
static idx_t  cache_cap = 0;    /* allocated capacity of cache */

/* Ensure that cache contains a[0] … a[n] */
static void ensure_cache(idx_t n)
{
    /* Lazy initialisation: store a(0)=a(1)=1 */
    if (cache_len == 0) {
        size_t init_cap = 4;
        cache = (seq_t*)malloc(init_cap * sizeof(seq_t));
        cache_cap = (idx_t)init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;                     /* we have a[0] and a[1] */
    }

    /* Expand array if necessary */
    if (cache_cap <= n) {
        size_t newcap = (cache_cap == 0) ? 4 : cache_cap;
        while (newcap <= n) newcap <<= 1;
        cache = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        cache_cap = (idx_t)newcap;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 2) {          /* already initialised */
            continue;
        }
        seq_t sum = 0;
        seq_t binom = 1;      /* binomial(i-2, 0) */
        seq_t pow   = 1;      /* (-3)^0 */
        idx_t kmax  = i - 2;

        for (idx_t k = 0; k <= kmax; ++k) {
            seq_t term = binom * pow;
            term *= cache[i - k - 2];   /* a(i-k-2) */
            sum += term;

            if (k < kmax) {
                binom = binom * (seq_t)(i - 2 - k);
                binom /= (seq_t)(k + 1); /* exact division */
                pow *= -(seq_t)3;
            }
        }
        cache[i] = sum;
    }
    cache_len = n + 1;   /* now a[0] … a[n] are stored */
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
