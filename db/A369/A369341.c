/*
 * A369341  Number of compositions (ordered partitions) of n into triangular
 *          numbers not greater than sqrt(n).
 * https://oeis.org/A369341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of compositions of n into triangular numbers not
 *            greater than floor(sqrt(n))
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root for a 64‑bit unsigned integer. */
static inline idx_t
isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t high = 1;
    while ((__int128)high * high <= n)
        high <<= 1;
    idx_t low = high >> 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if ((__int128)mid * mid <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Memoized cache of computed terms. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

/* Temporary DP buffer used for each call. */
static seq_t *dp_buf = NULL;
static idx_t dp_cap = 0;

/* Helper to ensure the cache can hold index n. */
static void
ensure_cache(idx_t n)
{
    if (n < cache_cap) return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 1;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) exit(EXIT_FAILURE);
    cache = new_cache;
    /* New entries are zero (uncomputed). */
    for (idx_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
    cache_cap = new_cap;
}

/* Helper to ensure the DP buffer can hold index n. */
static void
ensure_dp(idx_t n)
{
    idx_t need = n + 1;
    if ((size_t)need <= dp_cap) return;
    idx_t new_cap = dp_cap ? dp_cap << 1 : 1;
    while ((size_t)new_cap < need) new_cap <<= 1;
    seq_t *new_buf = (seq_t *)realloc(dp_buf, new_cap * sizeof(seq_t));
    if (!new_buf) exit(EXIT_FAILURE);
    dp_buf = new_buf;
    dp_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Hit cache first. */
    ensure_cache(n);
    if (cache[n] != 0) return cache[n];

    /* Compute the term anew. */
    idx_t M = isqrt_u64(n);
    /* Triangle numbers ≤ M.  They fit easily in a small array. */
    idx_t tri[128];
    idx_t tri_cnt = 0;
    for (idx_t k = 1; ; ++k) {
        idx_t t = k * (k + 1) / 2;
        if (t > M) break;
        tri[tri_cnt++] = t;
    }

    ensure_dp(n);
    dp_buf[0] = 1;          /* base case */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t j = 0; j < tri_cnt && tri[j] <= i; ++j)
            sum += dp_buf[i - tri[j]];
        dp_buf[i] = sum;
    }

    cache[n] = dp_buf[n];
    return dp_buf[n];
}
