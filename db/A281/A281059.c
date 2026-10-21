/*
 * A281059  Number of 4Xn 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly two elements, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical linear recurrence for A281059 * * a(1) = 0 * a(2) = 29
 *            * a(3) = 376 * a(4) = 1022 * a(5) = 2452 * a(6) = 5400 * a(7) =
 *            10570 * a(8) = 19892 * a(9) = 36616 * a(10) = 66354 * a(11) =
 *            118926 * a(12) = 211382 * a(13) = 373266 * a(14) = 655580 *
 *            a(15) = 1146156 * a(16) = 1995858 * a(17) = 34...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 30, u64 n <= 73, u128 n <= 163,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* external integer types supplied by the build */
extern seq_t a(idx_t n)
{
    /* static memoisation array: indices 1..size are valid */
    static seq_t *cache = NULL;          /* cache[1..size] */
    static idx_t cache_size = 0;         /* largest index currently stored */

    /* initialise cache on first call */
    if (!cache) {
        /* allocate space for indices 0..17 (index 0 unused) */
        cache = (seq_t *)malloc(18 * sizeof(seq_t));
        if (!cache) return 0;            /* out of memory (should never happen) */

        /* store initial values */
        cache[0] = 0;                     /* unused, keeps 1‑based indexing simple */
        cache[1] = 0;
        cache[2] = 29;
        cache[3] = 376;
        cache[4] = 1022;
        cache[5] = 2452;
        cache[6] = 5400;
        cache[7] = 10570;
        cache[8] = 19892;
        cache[9] = 36616;
        cache[10] = 66354;
        cache[11] = 118926;
        cache[12] = 211382;
        cache[13] = 373266;
        cache[14] = 655580;
        cache[15] = 1146156;
        cache[16] = 1995858;
        cache[17] = 3463242;
        cache_size = 17;
    }

    /* answer immediately if already computed */
    if (n <= cache_size)
        return cache[n];

    /* grow cache to hold result for n */
    size_t new_size = (size_t)n + 1;      /* indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;                   /* out of memory (should never happen) */
    cache = tmp;

    /* compute missing values from cache_size+1 up to n */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        /* recurrence valid only for i > 17 */
        if (i <= 17) continue;            /* safety check; should not happen */

        seq_t val =
            6 * cache[i - 1]
          - 12 * cache[i - 2]
          + 5 * cache[i - 3]
          + 12 * cache[i - 4]
          - 12 * cache[i - 5]
          - 3 * cache[i - 6]
          + 6 * cache[i - 7]
          - cache[i - 9];
        cache[i] = val;
    }

    cache_size = n;
    return cache[n];
}
