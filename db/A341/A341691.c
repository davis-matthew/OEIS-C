/*
 * A341691  a(0) = 0, and for any n > 0, a(n) = n - a(k) where k is the
 *          greatest number < n such that n AND a(k) = a(k) (where AND denotes
 *          the bitwise AND operator).
 * https://oeis.org/A341691
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0)=0 For n>0: a(n)=n - a(k) where k is the greatest number < n
 *            satisfying (n & a(k)) == a(k) (bitwise AND). The sequence can be
 *            generated incrementally and cached
 * Range:     u8 n <= 255, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External types provided by the build environment */
extern seq_t a(idx_t n);

/* Internal cache of previously computed a(i) values.
   cache[i] holds a(i) for 0 <= i < cache_size. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of computed terms */

static seq_t *
grow_cache(size_t new_size)
{
    /* Reallocate the cache to hold at least new_size elements. */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();           /* unrecoverable allocation failure */
    cache = tmp;
    return cache;
}

static void
ensure_cache_size(idx_t n)
{
    /* Grow the cache to include a(n) if necessary. */
    while (cache_size <= (size_t)n) {
        size_t m = cache_size;                 /* index of next term to compute */
        grow_cache(m + 1);                     /* extend array by one element */

        /* Find the greatest k < m such that (m & a(k)) == a(k). */
        idx_t k;
        for (k = (idx_t)(m - 1); ; --k) {
            /* Compare using 64‑bit integers; a(k) <= k <= m. */
            if ( ( ((idx_t)m & (idx_t)cache[(size_t)k]) == (idx_t)cache[(size_t)k]) ) {
                cache[(size_t)m] = (seq_t)m - cache[(size_t)k];
                break;
            }
            if (k == 0) {                 /* fall back (should never occur) */
                cache[(size_t)m] = (seq_t)m;
                break;
            }
        }

        ++cache_size;   /* a(m) has now been stored */
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)   /* base case */
        return 0;

    /* Initialise cache with a(0) on first call. */
    if (cache_size == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) abort();            /* allocation failure */
        cache[0] = 0;
        cache_size = 1;
    }

    /* Compute values up to n if necessary. */
    ensure_cache_size(n);

    return cache[(size_t)n];
}
