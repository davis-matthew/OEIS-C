/*
 * A395193  Area of the n-th convex polyiamond formed by winding spirals of
 *          incrementing strips of polyiamonds.
 * https://oeis.org/A395193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 195*(a(n-5) - a(n-10)) + a(n-15); Initial values: a(1) =
 *            1, a(2) = 3, a(3) = 6, a(4) = 10, a(5) = 66, a(6) = 253, a(7) =
 *            703, a(8) = 1176, a(9) = 2016, a(10) = 12880, a(11) = 49141,
 *            a(12) = 136503, a(13) = 228150, a(14) = 391170, a(15) = 2498730
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if ((idx_t)0 == n) return (seq_t)0;           /* offset is 1, below irrelevant */

    size_t N = (size_t)n;                         /* n >= 1 */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static const seq_t init[16] = {
        0,          /* unused 0 index */
        1, 3, 6, 10, 66, 253, 703, 1176, 2016, 12880,
        49141, 136503, 228150, 391170, 2498730
    };

    if (cap == 0) {                               /* first call: create cache */
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        for (size_t i = 1; i < cap; ++i)
            cache[i] = init[i];
    }

    if (N >= cap) {                               /* enlarge to store up to N */
        size_t newcap = N + 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        for (size_t i = cap; i < newcap; ++i)
            cache[i] = (seq_t)0;                 /* mark as not computed */
        cap = newcap;
    }

    if (N <= 15) return cache[N];                 /* initial values known */

    /* Compute forward up to N if necessary */
    for (size_t i = 16; i <= N; ++i) {
        if (cache[i] == (seq_t)0) {               /* not yet computed */
            seq_t diff = cache[i - 5] - cache[i - 10];
            seq_t term = ((seq_t)195) * diff + cache[i - 15];
            cache[i] = term;
        }
    }
    return cache[N];
}
