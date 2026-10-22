/*
 * A385038  Consecutive internal states of the linear congruential
 *          pseudo-random number generator (1366*s + 1283) mod 6075 when
 *          started at 1.
 * https://oeis.org/A385038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (1366*a(n-1) + 1283) mod 6075, a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 6075, u32 n <= 6075, u64 n <= 6075,
 *            u128 n <= 6075, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define PERIOD 6075

seq_t a(idx_t n) {
    if (n < 1) return 0;   /* undefined for n < 1, but return 0 safely */

    /* Reduce n modulo the period; the sequence is periodic with period 6075. */
    idx_t idx = n % PERIOD;
    if (idx == 0) idx = PERIOD;  /* idx is now in 1..PERIOD */

    /* Static cache of one full period; build on first call. */
    static seq_t *cache = NULL;
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t) * PERIOD);
        if (!cache) exit(1);  /* allocation failed */

        seq_t val = 1;                /* a(1) */
        cache[0] = val;
        for (idx_t i = 1; i < PERIOD; ++i) {
            val = (val * 1366 + 1283) % 6075;  /* next term */
            cache[i] = val;
        }
    }
    return cache[idx - 1];
}
