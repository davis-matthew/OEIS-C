/*
 * A291511  The arithmetic function uhat(n,2,6).
 * https://oeis.org/A291511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -23 if n is odd and n < 8, -24 if n is even and n < 8,
 *            otherwise -3*n
 * Range:     i8 n <= 42, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 8) {
        /* n is unsigned, use n & 1 to test oddness */
        if (n & 1U)
            return (seq_t)-23;
        else
            return (seq_t)-24;
    }
    seq_t s = (seq_t)3 * (seq_t)n;
    return -s;
}
