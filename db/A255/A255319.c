/*
 * A255319  Expansion of psi(x^3) * f(x, x^5) in powers of x where psi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A255319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 236, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A255319: Expansion of ψ(x³) * f(x, x⁵)  
   ψ(q)  = Σ_{k≥0} q^{k(k+1)/2}
   f(a,b)= Σ_{n∈ℤ} a^{n(n+1)/2} b^{n(n-1)/2}
   Thus a(m) = #solutions to   3k(k+1)/2 + n(3n–2) = m,  k≥0, n∈ℤ   */
#include <stdlib.h>

/* Cached sequence values */
static seq_t *cache = NULL;   /* array of a(0)…a(max) */
static idx_t cache_len = 0;   /* length of cache (max+1) */

/* Build cache up to new_max (inclusive) */
static void build_cache(idx_t new_max)
{
    /* --- produce all possible A = 3k(k+1)/2 ≤ new_max --- */
    idx_t k = 0;
    seq_t a_val = 0;
    idx_t a_count = 0;
    while (1) {
        a_val = (seq_t)3 * k * (k + 1) / 2;
        if (a_val > new_max)
            break;
        ++a_count;
        ++k;
    }
    seq_t *A = (seq_t *)malloc(sizeof(seq_t) * a_count);
    k = 0;
    idx_t i = 0;
    while (1) {
        a_val = (seq_t)3 * k * (k + 1) / 2;
        if (a_val > new_max)
            break;
        A[i++] = a_val;
        ++k;
    }

    /* --- produce all possible B = n(3n–2) for n∈ℤ ≤ new_max --- */
    idx_t b_count = 1;           /* for n = 0 */
    idx_t n = 1;
    seq_t b_val;
    while (1) {                  /* n > 0 */
        b_val = (seq_t)n * ((seq_t)3 * n - 2);
        if (b_val > new_max)
            break;
        ++b_count;
        ++n;
    }
    idx_t t = 1;
    while (1) {                  /* n = -t < 0 */
        b_val = (seq_t)t * ((seq_t)3 * t + 2);
        if (b_val > new_max)
            break;
        ++b_count;
        ++t;
    }
    seq_t *B = (seq_t *)malloc(sizeof(seq_t) * b_count);
    i = 0;
    B[i++] = 0;                   /* n = 0 */
    n = 1;
    while (1) {                  /* positive n */
        b_val = (seq_t)n * ((seq_t)3 * n - 2);
        if (b_val > new_max)
            break;
        B[i++] = b_val;
        ++n;
    }
    t = 1;
    while (1) {                  /* negative n */
        b_val = (seq_t)t * ((seq_t)3 * t + 2);
        if (b_val > new_max)
            break;
        B[i++] = b_val;
        ++t;
    }

    /* --- convolution: a(m) = #solutions A + B = m --- */
    seq_t *new_cache = (seq_t *)calloc((size_t)new_max + 1, sizeof(seq_t));
    for (i = 0; i < (idx_t)a_count; ++i) {
        seq_t a = A[i];
        for (idx_t j = 0; j < (idx_t)b_count; ++j) {
            seq_t b = B[j];
            idx_t sum = (idx_t)a + (idx_t)b;
            if (sum <= new_max)
                ++new_cache[sum];
        }
    }

    /* replace old cache */
    free(cache);
    free(A);
    free(B);
    cache = new_cache;
    cache_len = new_max + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (cache == NULL || n >= cache_len)
        build_cache(n);
    return cache[n];
}
