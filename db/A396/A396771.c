/*
 * A396771  Second least non-divisor of n.
 * https://oeis.org/A396771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the second least positive integer that does not divide
 *            n. The function searches for the first two integers m > 1 that
 *            are not divisors of n and returns the second one
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 2;; ++m) {
        if (n % m != 0) {
            ++count;
            if (count == 2)
                return (seq_t)m;
        }
    }
}
