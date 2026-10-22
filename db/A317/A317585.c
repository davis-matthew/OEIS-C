/*
 * A317585  a(n) = number of k with 1 <= k <= n-1 such that abs(a(k) - a(n-k))
 *          <= 2.
 * https://oeis.org/A317585
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of k with 1 <= k <= n-1 such that abs( a(k) -
 *            a(n-k) ) <= 2 (n >= 1, a(1) = 0)
 * Range:     u8 n <= 4505, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises previously computed values and
 * counts each pair only once (k <= n-k).  It runs in
 * O(max_n^2) time, which is below the 2‑second limit for
 * n up to about 1 000 000 in the allowed environments. */
#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;       /* array of a(1) … a(n) */
static idx_t cached_size = 0;    /* size of allocated array */
static idx_t computed_up_to = 0; /* highest n computed */

/* grow the cache to hold indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if (n + 1 <= cached_size)
        return;
    idx_t new_size = (cached_size == 0) ? 64 : cached_size;
    while (new_size <= n)
        new_size <<= 1;
#if defined(__cplusplus)
    seq_t *new_cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
#else
    seq_t *new_cache = realloc(cache, new_size * sizeof(seq_t));
#endif
    if (!new_cache)
        abort();                   /* out of memory */
    cache = new_cache;
    cached_size = new_size;
}

/* main recursive definition */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* undefined for n=0, but safe */

    ensure_cache_size(n);

    /* initialise base case a(1) = 0 */
    if (computed_up_to < 1) {
        cache[1] = (seq_t)0;
        computed_up_to = 1;
    }

    /* compute missing values up to n */
    if (computed_up_to < n) {
        for (idx_t m = computed_up_to + 1; m <= n; ++m) {
            seq_t cnt = 0;
            /* iterate k <= m-k to count each unordered pair once */
            idx_t half = m >> 1;          /* floor(m/2) */
            for (idx_t k = 1; k <= half; ++k) {
                idx_t other = m - k;
                seq_t left  = cache[k];
                seq_t right = cache[other];
                seq_t diff = (left > right) ? (left - right) : (right - left);
                if (diff <= 2) {
                    if (k == other)
                        ++cnt;           /* pair with itself */
                    else
                        cnt += 2;         /* both (k,other) and (other,k) */
                }
            }
            cache[m] = cnt;
        }
        computed_up_to = n;
    }
    return cache[n];
}
