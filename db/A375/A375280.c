/*
 * A375280  Largest value in the trajectory of n in the A375265 map.
 * https://oeis.org/A375280
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max of the trajectory of n under the A375265 map (x/3 if
 *            3|x, else x/2 if 2|x, else 3x+1) until 1 is reached; computed by
 *            direct iteration
 * Range:     u8 n <= 30, u16 n <= 702, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 8512 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t best = x;
    while (x != (seq_t)1) {
        if (x % (seq_t)3 == (seq_t)0)
            x = x / (seq_t)3;
        else if (x % (seq_t)2 == (seq_t)0)
            x = x / (seq_t)2;
        else
            x = x * (seq_t)3 + (seq_t)1;
        if (x > best) best = x;
    }
    return best;
}
