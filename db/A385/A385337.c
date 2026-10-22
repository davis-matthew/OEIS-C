/*
 * A385337  Consecutive states of the linear congruential pseudo-random number
 *          generator (1291*s + 4621) mod 21870 when started at s=1.
 * https://oeis.org/A385337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look, easy
 * Formula:   a(n) = (1291 * a(n-1) + 4621) mod 21870 a(1) = 1
 * Range:     u8 n <= 1, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)1;          /* a(1) */
    seq_t val = (seq_t)1;                  /* current value a(i) */
    for (idx_t i = 2; i <= n; ++i) {
        val = ((seq_t)1291 * val + (seq_t)4621) % (seq_t)21870;
    }
    return val;
}
