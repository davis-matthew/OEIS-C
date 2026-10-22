/*
 * A385358  Consecutive states of the linear congruential pseudo-random number
 *          generator (741*s + 66037) mod 312500 when started at s=1.
 * https://oeis.org/A385358
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = (741 * a(n-1) + 66037) mod 312500 (n > 1)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* period of the LCG sequence */
    static const idx_t PERIOD = 312500U;

    /* static cache: store first PERIOD terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* initialize cache on first call */
    if (cache == NULL) {
        cache = (seq_t *)malloc(PERIOD * sizeof(seq_t));
        if (cache == NULL) return 0;            /* handle allocation failure */
        cache[0] = (seq_t)1;                    /* a(1) */

        for (idx_t i = 1; i < PERIOD; ++i) {
            /* compute a(i+1) from a(i) */
            cache[i] = ((seq_t)741 * cache[i - 1] + 66037) % 312500;
        }
        cache_size = PERIOD;
    }

    /* map n to the 1‑based index within one period */
    idx_t idx = (n - 1) % cache_size;   /* idx in [0, PERIOD-1] */

    return cache[idx];
}
