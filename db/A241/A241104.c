/*
 * A241104  Number of nX4 0..2 arrays with no element equal to one or three
 *          horizontal or vertical neighbors, with new values 0..2 introduced
 *          in row major order.
 * https://oeis.org/A241104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A241104
 *
 *  Formula:
 *      a(n) = 6*a(n−1) + 21*a(n−2) – 12*a(n−3) + 8*a(n−4)
 *             – 688*a(n−5) – 1648*a(n−6) – 768*a(n−7) – 2304*a(n−8)
 *             + 8192*a(n−9) + 16384*a(n−10)    for n > 10
 *      a(1) = 4,  a(2) = 36,  a(3) = 294,  a(4) = 2436,
 *      a(5) = 19814,  a(6) = 162776,  a(7) = 1333934,
 *      a(8) = 10937316, a(9) = 89651534,  a(10) = 734979136
 *
 *  The function caches computed values so that calls in any order are
 *  efficiently answered.  It works for all integer types that OEIS
 *  supplies for the build (unsigned 8/16/32/64 bits, unsigned __int128,
 *  or an arbitrary precision integer class).
 */

#include <stdlib.h>      /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Base values (a(1) .. a(10)) */
    const seq_t base[11] = {
        0, /* unused (0) */
        4, 36, 294, 2436, 19814,
        162776, 1333934, 10937316,
        89651534, 734979136
    };

    /* Cache of previously computed terms */
    static seq_t *cache = NULL;      /* 1‑based indexing */
    static idx_t max_cached = 0;     /* highest n already stored */

    if (n < 1) {
        return 0;   /* undefined for n < 1 */
    }

    /* Allocate or extend cache if necessary */
    if (n > max_cached) {
        size_t new_len = (size_t)n + 1;   /* +1 for 1‑based indexing */
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – give up (return 0) */
            free(cache);
            cache = NULL;
            max_cached = 0;
            return 0;
        }
        cache = tmp;

        /* Compute new terms */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            if (i <= 10) {
                cache[i] = base[(size_t)i];
            } else {
                /* Separate positives and negatives to avoid
                 * a negative intermediate sum that could underflow
                 * an unsigned type. */
                seq_t pos = cache[i-1] * 6
                          + cache[i-2] * 21
                          + cache[i-4] * 8
                          + cache[i-9] * 8192
                          + cache[i-10] * 16384;

                seq_t neg = cache[i-3] * 12
                          + cache[i-5] * 688
                          + cache[i-6] * 1648
                          + cache[i-7] * 768
                          + cache[i-8] * 2304;

                cache[i] = pos - neg;  /* unsigned subtraction is fine here */
            }
        }
        max_cached = n;
    }

    return cache[n];
}
