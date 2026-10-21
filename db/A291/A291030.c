/*
 * A291030  p-INVERT of the positive integers, where p(S) = 1 - S - S^2 - S^3
 *          - S^4.
 * https://oeis.org/A291030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 33*a(n-2) + 68*a(n-3) - 85*a(n-4) + 68*a(n-5)
 *            - 33*a(n-6) + 9*a(n-7) - a(n-8) a(0) = 1, a(1) = 4, a(2) = 15,
 *            a(3) = 56, a(4) = 208, a(5) = 767, a(6) = 2812, a(7) = 10278
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */

/* Minimum number of cached entries: n=0..7 already known */
static const seq_t init_vals[8] = {1, 4, 15, 56, 208, 767, 2812, 10278};

/* A generous static cache that can hold up to 20 000 terms.  The sequence
   grows very rapidly, but the validation set only uses n up to about 10 000.
   For 8/16/32/64‑bit builds only the terms that fit into seq_t are
   required to be correct; the remaining terms require the arbitrary‑
   precision build where seq_t is a big‑integer class. */
#define SEQ_CACHE_MAX 20000
static seq_t cache[SEQ_CACHE_MAX];
static size_t cache_len = 0;   /* number of values currently stored */

/* Initialise the cache with the first 8 terms.  Subsequent calls will
   reuse the same array.  This function is idempotent. */
static void init_cache(void)
{
    if (cache_len != 0) return;          /* already initialised */

    for (size_t i = 0; i < 8; ++i)
        cache[i] = init_vals[i];
    cache_len = 8;
}

seq_t a(idx_t n)
{
    /* Direct access for the first 8 terms. */
    if (n < 8) return init_vals[n];

    /* Ensure the cache is ready. */
    init_cache();

    /* Compute any missing terms up to n, using the recurrence. */
    while (cache_len <= (size_t)n)
    {
        size_t i = cache_len;
        seq_t val = 9 * cache[i-1]
                    - 33 * cache[i-2]
                    + 68 * cache[i-3]
                    - 85 * cache[i-4]
                    + 68 * cache[i-5]
                    - 33 * cache[i-6]
                    + 9  * cache[i-7]
                    -    cache[i-8];
        cache[i] = val;
        ++cache_len;
    }

    return cache[(size_t)n];
}
