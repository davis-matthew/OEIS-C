/*
 * A385003  Consecutive states of the linear congruential pseudo-random number
 *          generator (421*s + 1663) mod 7875 when started at s=1.
 * https://oeis.org/A385003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(1)=1; a(n) = (421*a(n-1)+1663) mod 7875
 * Range:     u8 n <= 1, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

#define PERIOD 7875

seq_t a(idx_t n)
{
    static seq_t cache[PERIOD];
    static bool initialized = false;

    if (!initialized) {
        cache[0] = (seq_t)1;
        for (size_t i = 1; i < PERIOD; ++i) {
            seq_t prev = cache[i - 1];
            cache[i] = ((seq_t)421 * prev + (seq_t)1663) % (seq_t)PERIOD;
        }
        initialized = true;
    }

    idx_t idx = (n - 1) % PERIOD;
    return cache[(size_t)idx];
}
