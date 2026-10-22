/*
 * A364043  Expansion of Sum_{k>0} x^k / (1 + x^(5*k)).
 * https://oeis.org/A364043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -Sum_{d|n, d==1 (mod 5)} (-1)^d
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is computed by enumerating divisors d of n.  The sum
   is taken over divisors that are congruent to 1 modulo 5,
   adding +1 if d is even and -1 if d is odd.  The final
   sequence value is the negative of that sum. */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* sequence starts at offset 1; index 0 is unused */

    seq_t sum = 0;
    idx_t d;

    for (d = 1; d * d <= n; ++d) {
        if (n % d != 0)
            continue;

        /* first divisor d */
        if (d % 5 == 1) {
            sum += ((d & 1) ? (seq_t)-1 : (seq_t)1);
        }

        /* paired divisor n/d */
        idx_t d2 = n / d;
        if (d2 != d && d2 % 5 == 1) {
            sum += ((d2 & 1) ? (seq_t)-1 : (seq_t)1);
        }
    }

    return -sum;
}
