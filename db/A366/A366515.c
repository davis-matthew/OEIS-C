/*
 * A366515  Number of independent vertex sets in the n-Goldberg graph.
 * https://oeis.org/A366515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula from OEIS A366515:  a(n) = 27*a(n-1) + 227*a(n-2) - 1679*a(n-3)
 *                               + 36*a(n-4) + 4614*a(n-5) + 600*a(n-6)
 *                               - 1008*a(n-7) - 144*a(n-8)     (n ≥ 9)
 * Initial values: a(1) = 27, a(2) = 1183, a(3) = 33033, a(4) = 1115243,
 *                 a(5) = 35647837, a(6) = 1160360119, a(7) = 37555945909,
 *                 a(8) = 1217752812675
 */
#include <stdlib.h>

/* cache for already computed terms, 0‑based index: cache[i] == a(i+1) */
static seq_t *cache = NULL;
static idx_t cached_last = 0;          /* largest n cached (0 if none) */

/* initial 8 terms */
static const seq_t init[8] = {
    (seq_t)27,
    (seq_t)1183,
    (seq_t)33033,
    (seq_t)1115243,
    (seq_t)35647837,
    (seq_t)1160360119,
    (seq_t)37555945909,
    (seq_t)1217752812675
};

seq_t a(idx_t n)
{
    if (n < 1)                /* undefined for n < 1 */
        return 0;

    /* return cached value if available */
    if (cache && n <= cached_last)
        return cache[n - 1];

    /* enlarge cache if necessary */
    if (!cache || n > cached_last) {
        idx_t new_size = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)          /* out of memory, caller will see 0 */
            return 0;
        cache = tmp;
        /* zero newly allocated space */
        for (idx_t i = cached_last; i < new_size; ++i)
            cache[i] = 0;
    }

    /* compute terms from the next one after the cached range */
    idx_t start = (cached_last == 0) ? 1 : cached_last + 1;

    for (idx_t i = start; i <= n; ++i) {
        seq_t val;
        if (i <= 8) {
            val = init[i - 1];
        } else {
            /* indices are 1‑based in the recurrence:
             * a(i-1) -> cache[i-2], a(i-2) -> cache[i-3], … , a(i-8) -> cache[i-9]
             */
            seq_t a1 = cache[i - 2];
            seq_t a2 = cache[i - 3];
            seq_t a3 = cache[i - 4];
            seq_t a4 = cache[i - 5];
            seq_t a5 = cache[i - 6];
            seq_t a6 = cache[i - 7];
            seq_t a7 = cache[i - 8];
            seq_t a8 = cache[i - 9];

            val = (seq_t)27 * a1 + (seq_t)227 * a2
                  - (seq_t)1679 * a3
                  + (seq_t)36 * a4
                  + (seq_t)4614 * a5
                  + (seq_t)600 * a6
                  - (seq_t)1008 * a7
                  - (seq_t)144 * a8;
        }
        cache[i - 1] = val;
    }

    cached_last = n;
    return cache[n - 1];
}
