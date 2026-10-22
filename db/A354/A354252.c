/*
 * A354252  Expansion of e.g.f. 1/sqrt(7 - 6 * exp(x)).
 * https://oeis.org/A354252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=1..n} (6*n - 3*k) * C(n,k) * a(n-k) / n (n
 *            ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t last_computed = 0;

static void ensure_cache(idx_t n)
{
    if (cache_len > n) return;
    idx_t new_len = n + 1;
    seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
    if (!new_cache) exit(1);  /* allocation failure */
    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }
    /* initialize new entries to zero */
    for (idx_t i = cache_len; i < new_len; ++i)
        new_cache[i] = 0;
    cache = new_cache;
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    /* first call – initialise cache */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 1;
        cache_len = 1;
        last_computed = 0;
    }

    if (n >= cache_len) ensure_cache(n);

    if (n <= last_computed)
        return cache[n];

    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        seq_t bin = 1;   /* C(i,0) = 1 */
        for (idx_t k = 1; k <= i; ++k) {
            /* update bin = C(i,k) */
            bin = bin * (i - k + 1) / k;
            idx_t m = 6 * i - 3 * k;  /* 6*i - 3*k  (non‑negative) */
            seq_t term = bin * cache[i - k];  /* C(i,k)*a(i-k) */
            term = term * (seq_t)m;          /* multiply by m */
            term = term / (seq_t)i;          /* divide by i  – exact */
            sum += term;
        }
        cache[i] = sum;
    }
    last_computed = n;
    return cache[n];
}
