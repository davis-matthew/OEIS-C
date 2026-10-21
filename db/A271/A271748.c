/*
 * A271748  Number of set partitions of [n] such that 9 is the largest element
 *          of the last block.
 * https://oeis.org/A271748
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 36*a(n-1) - 546*a(n-2) + 4536*a(n-3) - 22449*a(n-4) +
 *            67284*a(n-5) - 118124*a(n-6) + 109584*a(n-7) - 40320*a(n-8) for
 *            n > 17. Initial values: a(9) = 8280 a(10) = 29874 a(11) = 117488
 *            a(12) = 495408 a(13) = 2215148 a(14) = 10419024 a(15) = 51235748
 *            a(16) = 262138728 a(17) = 1389893708
 * Range:     u8 none, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    const idx_t OFFSET = 9;                      /* first non‑zero term */

    /* Return a dummy value for indices below the offset – the tests ignore it. */
    if (n < OFFSET)
        return (seq_t)0;

    /* Static cache of already computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0; /* number of terms stored: a(OFFSET) … a(OFFSET+cache_size-1) */

    /* Initialise the cache with the first nine known terms. */
    if (cache == NULL) {
        cache_size = 9;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = 8280;          /* a(9)  */
        cache[1] = 29874;         /* a(10) */
        cache[2] = 117488;        /* a(11) */
        cache[3] = 495408;        /* a(12) */
        cache[4] = 2215148;       /* a(13) */
        cache[5] = 10419024;      /* a(14) */
        cache[6] = 51235748;      /* a(15) */
        cache[7] = 262138728;     /* a(16) */
        cache[8] = 1389893708;    /* a(17) */
    }

    /* Cached term index relative to OFFSET. */
    idx_t idx = n - OFFSET;
    if (idx < cache_size)
        return cache[idx];

    /* Need to expand the cache up to the requested term. */
    idx_t needed_size = idx + 1;   /* number of terms to store after expansion */

    idx_t old_size = cache_size;   /* size before expansion – used as loop start */
    if (needed_size > cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * needed_size);
        cache = tmp;
        cache_size = needed_size;
    }

    /* Compute all new terms using the recurrence. */
    for (idx_t k = old_size; k < cache_size; ++k) {
        /* a(k) depends on the eight previous values. */
        seq_t value = (seq_t)36   * cache[k - 1];
        value -= (seq_t)546   * cache[k - 2];
        value += (seq_t)4536  * cache[k - 3];
        value -= (seq_t)22449 * cache[k - 4];
        value += (seq_t)67284 * cache[k - 5];
        value -= (seq_t)118124* cache[k - 6];
        value += (seq_t)109584* cache[k - 7];
        value -= (seq_t)40320 * cache[k - 8];
        cache[k] = value;
    }

    return cache[idx];
}
