/*
 * A282015  Coefficients in q-expansion of E_4^5, where E_4 is the Eisenstein
 *          series shown in A004009.
 * https://oeis.org/A282015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in (1 + 240 * Sum_{i>=1} i^3 *
 *            q^i/(1-q^i))^5 where the constant term a(0) = 1. This implements
 *            the series expansion using the divisor–sum σ₃(m)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 11, u128 n <= 122,
 *            big any n
 * Helpers:   alloc_seq_size
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_seq_size.h"

static inline void free_seq_arr(seq_t *arr)
{
#ifdef __cplusplus
    delete[] arr;
#else
    free(arr);
#endif
}

static seq_t *sigma3   = NULL;   /* σ₃[0 .. N]  */
static seq_t *a_cache  = NULL;   /* a[0 .. N]    */
static idx_t  cache_sz = 0;      /* current maximum index stored */

/* Weight factors  C(5,k)·240^k  for k = 0..5 (k=0 gives 1) */
static const seq_t weight[6] = {
    (seq_t)1,                       /* k = 0   */
    (seq_t)1200,                    /* k = 1   = 5·240 */
    (seq_t)576000,                  /* k = 2   =10·240²   */
    (seq_t)138240000,               /* k = 3   =10·240³   */
    (seq_t)16588800000ULL,          /* k = 4   = 5·240⁴   */
    (seq_t)796262400000ULL          /* k = 5   =   240⁵   */
};

/* ------------------------------------------------------------------ */
/* Recompute all required tables up to index *n* (inclusive). */
static void ensure_up_to(idx_t n)
{
    if (cache_sz > n) return;        /* already prepared */

    size_t sz = (size_t)(n + 1);

    /* Allocate new arrays for σ₃ and a. */
    seq_t *new_sigma = alloc_seq_size(sz);
    seq_t *new_a     = alloc_seq_size(sz);

    /* Compute σ₃(m) for 1 ≤ m ≤ n. */
    for (idx_t d = 1; d <= n; ++d) {
        seq_t d3 = (seq_t)d * d * d;
        for (idx_t m = d; m <= n; m += d)
            new_sigma[m] += d3;
    }

    /* Allocate temporary convolution tables conv[k][0..n]. */
    seq_t *conv[6];
    for (int k = 0; k <= 5; ++k)
        conv[k] = alloc_seq_size(sz);

    /* Base cases */
    conv[0][0] = (seq_t)1;            /* (1)^0 = 1      */
    for (idx_t i = 1; i <= n; ++i)
        conv[1][i] = new_sigma[i];    /* g(q)            */

    /* Higher powers: conv[k] = convolution(conv[1], conv[k-1]) */
    for (int k = 2; k <= 5; ++k) {
        for (idx_t i = 1; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j < i; ++j)
                sum += new_sigma[j] * conv[k-1][i - j];
            conv[k][i] = sum;
        }
    }

    /* Compute a(n) for 0..n */
    new_a[0] = (seq_t)1;              /* a(0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        for (int k = 1; k <= 5; ++k)
            val += weight[k] * conv[k][i];
        new_a[i] = val;
    }

    /* Free old tables */
    if (sigma3) free_seq_arr(sigma3);
    if (a_cache) free_seq_arr(a_cache);
    for (int k = 0; k <= 5; ++k) free_seq_arr(conv[k]);

    /* Store new tables */
    sigma3   = new_sigma;
    a_cache  = new_a;
    cache_sz = sz;
}

/* ------------------------------------------------------------------ */
/* Public function required by the build system. */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return a_cache[n];
}
