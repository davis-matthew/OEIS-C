/*
 * A375393  a(0) = 1; a(n) = Sum_{k=0..n-1} (4*k+3) * a(k) * a(n-k-1).
 * https://oeis.org/A375393
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = Σ_{k=0}^{n-1} (4*k+3) * a(k) * a(n-k-1), n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a_cache = NULL;      /* memoization cache */
static idx_t  a_cache_len = 0;     /* number of values stored (next index to compute) */
static size_t a_cache_cap = 0;     /* allocated capacity */

/* Ensure the cache can hold indices up to n (but a_cache_len is unchanged) */
static void _ensure_cache(idx_t n)
{
    if (a_cache_cap <= (size_t)n) {
        size_t new_cap = a_cache_cap ? a_cache_cap : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        a_cache      = tmp;
        a_cache_cap  = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Initialize base case if needed */
    if (!a_cache) {
        a_cache = (seq_t *)malloc(sizeof(seq_t));
        if (!a_cache) abort();
        a_cache[0] = 1;
        a_cache_cap = 1;
        a_cache_len = 1;
    }

    /* Compute values up to n if they are not yet cached */
    if (a_cache_len <= n) {
        _ensure_cache(n);
        idx_t start = a_cache_len;          /* first index to compute */
        for (idx_t i = start; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k < i; ++k) {
                seq_t term = ((seq_t)(4 * k + 3)) * a_cache[k] * a_cache[i - k - 1];
                sum += term;
            }
            a_cache[i] = sum;
        }
        a_cache_len = n + 1;
    }

    return a_cache[n];
}
