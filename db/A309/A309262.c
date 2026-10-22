/*
 * A309262  a(0) = 0, a(1) = 1, and for any n > 1, a(n) = Sum_{k > 1}
 *          a(floor(n/k^2)).
 * https://oeis.org/A309262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(n)=Σ_{k=2}^{⌊√n⌋} a(⌊n/k²⌋)
 * Range:     u8 n <= 5183, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Index and sequence value types are supplied externally. */

/* Integer square root without overflow: return ⌊√n⌋ */
static inline idx_t int_sqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 1, high = n / 2 + 1;          /* high ≥ √n for n≥2 */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;    /* mid > 0 */
        if (mid <= n / mid)                   /* mid*mid ≤ n without overflow */
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* Static cache for already computed values */
static seq_t *cache = NULL;        /* array holding a(i) for i < cache_size */
static idx_t cache_size = 0;       /* number of initialized elements */
static idx_t cache_cap   = 0;       /* allocated capacity */

/* Ensure that cache[0…n] are defined.  Values are computed sequentially. */
static void ensure_cache(idx_t n)
{
    if (cache_size > n) return;

    /* Grow capacity if necessary */
    if (cache_cap == 0) cache_cap = 64;
    while (cache_cap <= n) cache_cap <<= 1;

    seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    /* If realloc failed, terminate (no graceful handling required here) */
    cache = new_cache;

    /* Compute values from old cache_size up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else {
            idx_t kmax = int_sqrt_u64(i);
            seq_t sum = (seq_t)0;
            for (idx_t k = 2; k <= kmax; ++k) {
                idx_t idx = i / (k * k);   /* floor division */
                sum += cache[idx];
            }
            cache[i] = sum;
        }
    }
    cache_size = n + 1;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
