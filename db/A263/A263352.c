/*
 * A263352  Expansion of Product_{k>=1} 1/(1 - x^(2*k+3))^k.
 * https://oeis.org/A263352
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in Product_{k>=1} 1/(1 - x^(2*k+3))^k
 *            (OEIS A263352, offset 0)
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 129, u64 n <= 304,
 *            u128 n <= 759, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifndef SEQ_T_DEFINED
/* seq_t and idx_t are defined by the build system. */
#endif

/* Define allocation/deallocation helpers that work for both C and C++. */
#if defined(__cplusplus)
/* In C++ we can use new/delete for array of seq_t. */
static inline seq_t* alloc_seq(size_t sz) { return new seq_t[sz]; }
static inline void free_seq(seq_t* p) { delete[] p; }
#else
/* In plain C we use malloc/free. */
static inline seq_t* alloc_seq(size_t sz) { return (seq_t*)malloc(sz * sizeof(seq_t)); }
static inline void free_seq(seq_t* p) { free(p); }
#endif

/* Static cache for previously computed values. */
static seq_t* dp_a = NULL;   /* Current DP array */
static seq_t* dp_b = NULL;   /* Temporary array for convolution */
static idx_t   cache_n = 0;   /* Highest n for which dp_a is valid */

/* Recompute coefficients up to new_n (inclusive).  Frees any old cache. */
static void recompute_upto(idx_t new_n)
{
    size_t sz = (size_t)new_n + 1;
    seq_t* a = alloc_seq(sz);
    seq_t* b = alloc_seq(sz);

    if (!a || !b) {
        free_seq(a); free_seq(b);
        if (dp_a) free_seq(dp_a);
        if (dp_b) free_seq(dp_b);
        dp_a = dp_b = NULL;
        cache_n = 0;
        return;
    }

    /* initialise a[0] = 1, others 0 */
    for (size_t i = 0; i < sz; ++i) a[i] = (seq_t)0;
    a[0] = (seq_t)1;

    /* Convolve with each factor (1 - x^e)^(-k), e = 5,7,9,... ≤ new_n */
    for (idx_t e = 5; e <= new_n; e += 2) {
        idx_t k = (e - 3) / 2;            /* exponent of the factor   */
        idx_t max_m = new_n / e;          /* maximum multiplicity for this e */

        /* zero temporary array b */
        for (size_t i = 0; i < sz; ++i) b[i] = (seq_t)0;

        /* start with the current dp state */
        for (size_t i = 0; i < sz; ++i) b[i] = a[i];

        /* add contributions for m ≥ 1 */
        seq_t weight = (seq_t)k; /* binomial(k,1) */
        for (idx_t m = 1; m <= max_m; ++m) {
            size_t offset = (size_t)m * (size_t)e;
            for (size_t i = 0; i <= new_n - offset; ++i) {
                seq_t mult = a[i] * weight;
                b[i + offset] += mult;
            }
            if (m < max_m) {
                seq_t num = weight * (seq_t)(k + m);
                seq_t den = (seq_t)(m + 1);
                weight = num / den;   /* exact division */
            }
        }

        /* swap "current" and "temporary" arrays for next iteration */
        seq_t* tmp = a; a = b; b = tmp;
    }

    /* Replace old cache with the new one.  The old memory is freed. */
    if (dp_a) free_seq(dp_a);
    if (dp_b) free_seq(dp_b);
    dp_a = a;
    dp_b = b;
    cache_n = new_n;
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (!dp_a || n > cache_n) {
        recompute_upto(n);
    }
    return dp_a[n];
}
