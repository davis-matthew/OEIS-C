/*
 * A384113  Consecutive states of a linear congruential pseudo-random number
 *          generator for MacModula-2 when started at 1.
 * https://oeis.org/A384113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13 * a(n-1) mod 2311, with a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 1155, u32 n <= 1155, u64 n <= 1155,
 *            u128 n <= 1155, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define PERIOD 1155u

seq_t a(idx_t n)
{
    static seq_t cache[PERIOD];
    static int initialized = 0;

    if (!initialized) {
        cache[0] = (seq_t)1;
        for (idx_t i = 1; i < PERIOD; ++i) {
            /* 13*a(i-1) stays below 30030; a 32‑bit intermediate is enough */
            uint32_t prod = (uint32_t)cache[i - 1] * 13u;
            cache[i] = (seq_t)(prod % 2311u);
        }
        initialized = 1;
    }

    /* The sequence is periodic with period 1155. */
    idx_t pos = (n - 1u) % PERIOD;
    return cache[pos];
}
