/*
 * A370864  Numbers m such that c(0) >= c(1) < c(2), where c(k) = number of
 *          k's in the ternary representation of m.
 * https://oeis.org/A370864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Description:  a(n) is the nth positive integer m such that
   the number of zeros in the base‑3 representation of m is
   at least the number of ones, and the number of ones is less
   than the number of twos:
        c(0) >= c(1) < c(2) */
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 1;            /* start from 1 (offset 1) */

    while (1) {
        seq_t t = m;
        unsigned zeros = 0, ones = 0, twos = 0;

        /* Count digits of m in base 3 */
        while (!(t == (seq_t)0)) {
            seq_t r = t % (seq_t)3;
            if (r == (seq_t)0)
                zeros++;
            else if (r == (seq_t)1)
                ones++;
            else
                twos++;
            t = t / (seq_t)3;
        }

        if (zeros >= ones && ones < twos) {
            count++;
            if (count == n)
                return m;
        }
        m = m + (seq_t)1;
    }
}
