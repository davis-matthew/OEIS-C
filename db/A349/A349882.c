/*
 * A349882  Expansion of Sum_{k>=0} k^2 * x^k/(1 - k^2 * x).
 * https://oeis.org/A349882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} k^(2*(n-k+1))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Cached powers of k: powers[k] = k^{2*(current_n-k+1)}   (for k>=1) */
static seq_t   *powers      = NULL;
static size_t   powers_cap  = 0;
static idx_t    powers_last = 0;       /* maximum n for which powers are up-to-date */

/* Cached sequence values */
static seq_t   *cache       = NULL;
static size_t   cache_cap   = 0;
static idx_t    cache_last  = 0;

/* Current sum of powers for the current n */
static seq_t   current_sum = 0;

/* Ensure that arrays have storage for at least n+1 elements */
static void ensure_capacity(idx_t n)
{
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap == 0 ? 1 : cache_cap * 2;
        while (new_cap < (size_t)(n + 1)) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        /* initialize new entries to zero */
        if (tmp != cache) cache = tmp;
        memset(cache + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
        cache_cap = new_cap;
    }
    if (powers_cap <= (size_t)n) {
        size_t new_cap = powers_cap == 0 ? 1 : powers_cap * 2;
        while (new_cap < (size_t)(n + 1)) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(powers, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        if (tmp != powers) powers = tmp;
        memset(powers + powers_cap, 0, (new_cap - powers_cap) * sizeof(seq_t));
        powers_cap = new_cap;
    }
}

/* Compute sequence values up to n (inclusive) */
static void compute_up_to(idx_t n)
{
    if (n <= powers_last) return;

    for (idx_t cur = powers_last + 1; cur <= n; ++cur) {
        /* Update existing powers: multiply by k^2 */
        for (idx_t k = 1; k < cur; ++k) {
            seq_t old   = powers[k];
            seq_t kk    = (seq_t)k * (seq_t)k;
            seq_t new_v = old * kk;
            powers[k]   = new_v;
            current_sum += new_v - old;
        }
        /* New power for k=cur */
        seq_t kk = (seq_t)cur * (seq_t)cur;
        powers[cur] = kk;
        current_sum += kk;
        /* Store the result */
        cache[cur] = current_sum;
    }
    powers_last = n;
    cache_last = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* a(0) = 0 */

    /* At first call, ensure arrays exist and cache[0] = 0 */
    if (cache_cap == 0) {
        ensure_capacity(n);
        cache[0] = 0;
        cache_last = 0;
        powers_last = 0;
    } else {
        ensure_capacity(n);
    }

    compute_up_to(n);
    return cache[n];
}
