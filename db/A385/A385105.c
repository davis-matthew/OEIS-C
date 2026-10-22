/*
 * A385105  Harshad root of integer n: the largest integer produced by
 *          repeatedly dividing n by its digit sum until it fails to produce
 *          another integer or it reaches a power of 10.
 * https://oeis.org/A385105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Harshad root of n The root is the last integer obtained
 *            by repeatedly dividing by the digit sum; the process stops when
 *            the current number is a power of 10 (digit sum equals 1) or when
 *            it is not divisible by its digit sum
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    while (1) {
        seq_t ds = digit_sum(m);
        if (ds == 1) return m;          /* power of 10 (including 1) */
        if (m % ds != 0) return m;      /* no further division possible */
        m = m / ds;                     /* divide and continue */
    }
}
