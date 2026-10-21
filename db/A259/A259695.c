/*
 * A259695  a(n) = Sum_{k=1..n-1} k^7 * sigma(k) * sigma(n-k).
 * https://oeis.org/A259695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} k^7 * sigma(k) * sigma(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 13, u64 n <= 119, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cached divisor–sum values */
static seq_t *sigma_cache = NULL;
static idx_t sigma_cap = 0;

/* Build σ(i) for all 1 ≤ i ≤ n */
static void ensure_sigma(idx_t n)
{
    if (n <= sigma_cap)
        return;
    if (sigma_cache && sigma_cap > 0)
        free(sigma_cache);
    /* Allocate (n+1) entries and zero‑initialize them */
    sigma_cache = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    sigma_cap  = n;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t inc = (seq_t)i;
        for (idx_t j = i; j <= n; j += i)
            sigma_cache[j] += inc;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    ensure_sigma(n);
    seq_t total = (seq_t)0;
    for (idx_t k = 1; k < n; ++k) {
        seq_t kseq = (seq_t)k;
        /* Compute k^7 */
        seq_t kpow = kseq;
        for (int i = 1; i < 7; ++i)
            kpow *= kseq;
        seq_t term = kpow * sigma_cache[k] * sigma_cache[n - k];
        total += term;
    }
    return total;
}
