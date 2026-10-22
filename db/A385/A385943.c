/*
 * A385943  a(0) = 1; a(n) = Sum_{k=0..n-1} (1 + k) * (1 + k^6) *
 *          binomial(n-1,k) * a(k) * a(n-1-k).
 * https://oeis.org/A385943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = Sum_{k=0..n-1} (1 + k) * (1 + k^6) *
 *            binomial(n-1,k) * a(k) * a(n-1-k) (0‑indexed)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Multiplicative binomial coefficient: C(n,k) (assumed exact)     */
/* ------------------------------------------------------------------ */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* res = res * (n - k + i) / i  (exact division) */
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/*  Sequence a(n) with memoisation; uses static cache                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache   = NULL;
    static size_t  capa   = 0;      /* allocated capacity */
    static idx_t   last   = 0;      /* highest index computed */

    /* Ensure cache can hold at least a(0) */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;           /* a(0) = 1 */
        capa = 1;
        last = 0;
    }

    /* Grow cache if necessary */
    if ((size_t)n + 1 > capa) {
        size_t new_cap = capa ? capa : 1;
        while (new_cap < (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        capa = new_cap;
    }

    /* Compute any missing terms up to n (in any call order) */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            seq_t left   = cache[k];
            seq_t right  = cache[i - 1 - k];
            seq_t prod   = left * right;

            /* (1 + k) * (1 + k^6) */
            seq_t kk = (seq_t)k;
            seq_t k_pow6 = kk;
            for (int p = 0; p < 5; ++p) k_pow6 *= kk;   /* kk^6 */
            seq_t weight = (kk + (seq_t)1) * (k_pow6 + (seq_t)1);

            prod *= weight;
            prod *= binom(i - 1, k);

            sum += prod;
        }
        cache[i] = sum;
    }

    last = n;
    return cache[n];
}
