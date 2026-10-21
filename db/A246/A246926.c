/*
 * A246926  Expansion of phi(x)^2 * chi(x) * psi(-x^3) in powers of x where
 *          phi(), psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A246926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 479, u32 n <= 2500, u64 n <= 2500,
 *            u128 n <= 2500, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A246926
 *  a(n) = coefficient of x^n in φ(x)^2·χ(x)·ψ(–x³)
 *  The sequence is produced by the Euler‑transform recurrence:
 *  a(0)=1,
 *  for n≥1:  n·a(n) = Σ_{k=1..n} d(k)·a(n−k),
 *  where d(k)=Σ_{d|k} d·c(d) and c(d) is the period‑12 sequence
 *  [5, –7, 4, –2, 5, –7, 5, –2, 4, –7, 5, –3, …]
 *  This implementation keeps a cache of computed values for efficiency.
 */

#include <stdlib.h>
#include <stdint.h>

#if defined(__cplusplus)
/* For C++ builds (arbitrary‑precision seq_t), use dynamic construction. */
static seq_t* a_cache = nullptr;
static size_t a_cache_len = 0;
#else
/* For plain C builds (built‑in integer seq_t). */
static seq_t* a_cache = NULL;
static size_t a_cache_len = 0;
#endif

static void compute_upto(idx_t n)
{
    size_t len = (size_t)n + 1;

    /* Allocate cache of appropriate size, destroying any old one. */
#if defined(__cplusplus)
    if (a_cache_len)
        delete[] a_cache;
    a_cache = new seq_t[len];
#else
    if (a_cache)
        free(a_cache);
    a_cache = (seq_t*)malloc(len * sizeof(seq_t));
    if (!a_cache)
        exit(EXIT_FAILURE);
#endif
    a_cache_len = len;

    /* d(k) for k = 0..n (index 0 unused). */
    int64_t* d = (int64_t*)malloc(len * sizeof(int64_t));
    if (!d)
        exit(EXIT_FAILURE);
    for (size_t i = 0; i < len; ++i)
        d[i] = 0;

    static const int pattern[12] = {5, -7, 4, -2, 5, -7, 5, -2, 4, -7, 5, -3};

    for (size_t dval = 1; dval < len; ++dval) {
        int cval = pattern[(dval - 1) % 12];
        int64_t prod = (int64_t)dval * (int64_t)cval;
        for (size_t m = dval; m < len; m += dval)
            d[m] += prod;
    }

    /* Recurrence for a(n). */
    a_cache[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i; ++k)
            sum += a_cache[i - k] * (seq_t)d[k];
        a_cache[i] = sum / (seq_t)i;   /* exact division guaranteed. */
    }

    free(d);
}

seq_t a(idx_t n)
{
    if ((size_t)n + 1 > a_cache_len)
        compute_upto(n);
    return a_cache[(size_t)n];
}
