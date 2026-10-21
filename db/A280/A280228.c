/*
 * A280228  Number of n X 3 0..1 arrays with no element unequal to a strict
 *          majority of its king-move neighbors, with the exception of exactly
 *          one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A280228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2; a(2)=6; a(3)=9; a(4)=16; a(5)=29; a(6)=52; a(7)=95
 *            a(n)=2*a(n-1)+a(n-2)-2*a(n-3)-a(n-4) for n>7 Sequence A280228 –
 *            number of n×3 0/1 arrays with exactly one exception to the
 *            strict‑majority king‑move rule
 * Range:     u8 n <= 8, u16 n <= 18, u32 n <= 40, u64 n <= 85, u128 n <= 175,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* Global caching state */
#if defined(__cplusplus)
static std::vector<seq_t> cache_vec;       /* 1‑based storage, element 0 unused */
#else
static seq_t *cache = NULL;                /* 1‑based storage, element 0 unused */
#endif
static size_t cache_len = 0;               /* highest index that has a value */

/* Ensure that a(1)…a(n) are computed and stored. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_len)                /* nothing left to do */
        return;

    size_t required = (size_t)n + 1;   /* array length needed for indices 0…n */

#if defined(__cplusplus)
    /* Grow the vector as necessary */
    if (cache_vec.size() < required)
        cache_vec.resize(required);

    /* Initialise base values up to index n (but not beyond 7) */
    size_t base_limit = (n < 7 ? (size_t)n : 7);
    if (cache_len < base_limit) {
        if (base_limit >= 1) cache_vec[1] = (seq_t)2;
        if (base_limit >= 2) cache_vec[2] = (seq_t)6;
        if (base_limit >= 3) cache_vec[3] = (seq_t)9;
        if (base_limit >= 4) cache_vec[4] = (seq_t)16;
        if (base_limit >= 5) cache_vec[5] = (seq_t)29;
        if (base_limit >= 6) cache_vec[6] = (seq_t)52;
        if (base_limit >= 7) cache_vec[7] = (seq_t)95;
        cache_len = base_limit;
    }

    /* Compute the remaining terms using the recurrence */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        cache_vec[i] = (cache_vec[i-1] << 1) + cache_vec[i-2]
                       - (cache_vec[i-3] << 1) - cache_vec[i-4];
    }
    cache_len = (size_t)n;

#else   /* C implementation */

    seq_t *tmp = (seq_t*)realloc(cache, required * sizeof(seq_t));
    if (!tmp) return;                 /* allocation failed – give up */
    cache = tmp;

    size_t base_limit = (n < 7 ? (size_t)n : 7);
    if (cache_len < base_limit) {
        if (base_limit >= 1) cache[1] = (seq_t)2;
        if (base_limit >= 2) cache[2] = (seq_t)6;
        if (base_limit >= 3) cache[3] = (seq_t)9;
        if (base_limit >= 4) cache[4] = (seq_t)16;
        if (base_limit >= 5) cache[5] = (seq_t)29;
        if (base_limit >= 6) cache[6] = (seq_t)52;
        if (base_limit >= 7) cache[7] = (seq_t)95;
        cache_len = base_limit;
    }

    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        cache[i] = (cache[i-1] << 1) + cache[i-2]
                   - (cache[i-3] << 1) - cache[i-4];
    }
    cache_len = (size_t)n;

#endif
}

/* Public interface – compute a(n) with 1‑based indexing */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n<1 – return 0 */

    ensure_cache(n);
#if defined(__cplusplus)
    return cache_vec[(size_t)n];
#else
    return cache[(size_t)n];
#endif
}
