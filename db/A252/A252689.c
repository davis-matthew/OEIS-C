/*
 * A252689  Number of (n+2) X (2+2) 0..3 arrays with every consecutive three
 *          elements in every row and column having exactly two distinct
 *          values, and in every diagonal and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5 a(n-1) – 4 a(n-2) – 11 a(n-3) + 9 a(n-4) + 12 a(n-5) +
 *            12 a(n-6) – 34 a(n-7) – 8 a(n-8) + 28 a(n-9) – 8 a(n-10) with
 *            initial values a(1)=96, a(2)=142, a(3)=204, a(4)=519, a(5)=1047,
 *            a(6)=2719, a(7)=6483, a(8)=16887, a(9)=42825, a(10)=111655,
 *            a(11)=288975. This implements the recurrence f...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 90,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>      /* for size_t */
#include "../utils/grow_seq_array.h" /* grow_seq_array(seq_t **, size_t *, size_t) */

seq_t a(idx_t n)
{
    /* Initial values for n = 1 … 11 (offset 1). */
    static const seq_t init[] = {
        96ULL,     /* a(1) */
        142ULL,    /* a(2) */
        204ULL,    /* a(3) */
        519ULL,    /* a(4) */
        1047ULL,   /* a(5) */
        2719ULL,   /* a(6) */
        6483ULL,   /* a(7) */
        16887ULL,  /* a(8) */
        42825ULL,  /* a(9) */
        111655ULL, /* a(10) */
        288975ULL  /* a(11) */
    };
    const size_t init_len = sizeof(init)/sizeof(init[0]);

    /* Return an initial value directly if asked for n ≤ 11. */
    if (n <= init_len)
        return init[(size_t)(n - 1)];

    /* Dynamic cache of previously computed terms.
       The cache is 1‑based; index 0 is unused. */
    static seq_t *cache = NULL;     /* array of seq_t */
    static size_t cache_size = 0;   /* current allocated size (>= requested index+1) */
    static idx_t cached_max = 0;    /* highest index that has been computed */

    /* Ensure the cache is large enough to hold the first 12 entries
       (indices 0..11; index 0 is unused). */
    if (cache_size < init_len + 1) {
        grow_seq_array(&cache, &cache_size, init_len + 1);
        /* Initialize the known initial values. */
        for (size_t i = 1; i <= init_len; ++i)
            cache[i] = init[i - 1];
        cached_max = init_len;
    }

    /* Extend the cache up to index n if necessary. */
    if (cached_max < n) {
        grow_seq_array(&cache, &cache_size, (size_t)n + 1);
        /* Compute missing terms sequentially. */
        for (idx_t k = cached_max + 1; k <= n; ++k) {
            seq_t pos = 5 * cache[k - 1] + 9 * cache[k - 4] +
                        12 * cache[k - 5] + 12 * cache[k - 6] +
                        28 * cache[k - 9];
            seq_t neg = 4 * cache[k - 2] + 11 * cache[k - 3] +
                        34 * cache[k - 7] + 8 * cache[k - 8] +
                        8 * cache[k - 10];
            cache[k] = pos - neg;
        }
        cached_max = n;
    }

    return cache[n];
}
