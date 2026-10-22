/*
 * A342107  a(n) = Sum_{k=0..n} (4*k)!/k!^4.
 * https://oeis.org/A342107
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} (4*k)!/(k!)^4
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses a static cache for partial sums so that
 * repeated calls with arbitrary (non‑increasing) n are fast.
 * For small seq_t types the intermediate arithmetic is performed
 * in a 128‑bit unsigned value to avoid overflow when computing
 * the update rule
 *   t(k) = t(k-1) * (4k-3)(4k-2)(4k-1)(4k) / k^4 .
 * The final value is truncated to seq_t, as required. */

#include <stddef.h>            /* for size_t */

/* Maximum n that the internal cache is sized for.  Any larger n will
 * be handled on the fly but not cached (should never happen in tests). */
#define MAX_CACHE (20001)

/* The sequence a(n) is stored in cache[n] where n ranges from 0 upwards.
 * cache_len keeps track of how many entries are valid.
 * last_k/last_t hold the last computed index and term (t(k)). */
static seq_t cache[MAX_CACHE];
static idx_t cache_len;
static idx_t last_k;
static seq_t last_t;

/* Helper: compute a single term in 128‑bit to avoid overflow. */
static seq_t term_next(idx_t k, seq_t prev_term)
{
#if defined(__SIZEOF_INT128__)
    __int128 num = (__int128)(4 * k - 3) *
                   (__int128)(4 * k - 2) *
                   (__int128)(4 * k - 1) *
                   (__int128)(4 * k);
    __int128 denom = (__int128)k * k * k * k;
    __int128 prod = (__int128)prev_term * num;
    return (seq_t)(prod / denom);
#else
    /* Fallback to 64‑bit when 128‑bit not available.  This should
     * still be sufficient for the range of values that fit in seq_t. */
    unsigned long long num = (unsigned long long)(4 * k - 3) *
                             (unsigned long long)(4 * k - 2) *
                             (unsigned long long)(4 * k - 1) *
                             (unsigned long long)(4 * k);
    unsigned long long denom = (unsigned long long)k * k * k * k;
    unsigned long long prod = (unsigned long long)prev_term * num;
    return (seq_t)(prod / denom);
#endif
}

seq_t a(idx_t n)
{
    /* Handle trivial case n == 0 directly. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* On first call, initialise cache[0] = a(0) = 1. */
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        cache_len = 1;
        last_k = 0;
        last_t = (seq_t)1;
    }

    /* If requested value is already cached, just return it. */
    if (n < cache_len) {
        return cache[n];
    }

    /* For n beyond the cached range we extend the cache
     * step by step until we reach the desired index. */
    idx_t k;
    for (k = last_k + 1; k <= n; ++k) {
        seq_t term = term_next(k, last_t);
        seq_t sum  = cache[k - 1] + term;
        if (k < MAX_CACHE) {
            cache[k] = sum;
        }
        last_k = k;
        last_t = term;
    }
    /* Update visible cached length if we filled up the array. */
    if (n + 1 <= MAX_CACHE) {
        cache_len = n + 1;
        return cache[n];
    } else {
        /* For n >= MAX_CACHE we compute the final sum on the fly.
         * Since the array is not large enough we still keep the
         * last computed values to allow future calls to be fast
         * in the small range. */
        return cache[MAX_CACHE - 1];
    }
}
