/*
 * A243983  Sum of twin divisors of n.
 * https://oeis.org/A243983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of twin divisors of n; a divisor d of n is a twin
 *            divisor if either d+2 or |d-2| is also a divisor of n
 * Range:     u8 n <= 2519, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* iterate over all divisors d of n */
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            /* d is a divisor */
            if (d > 2 && n % (d - 2) == 0) {
                sum += (seq_t)d;
            } else if (n % (d + 2) == 0) {
                sum += (seq_t)d;
            }
            idx_t e = n / d;
            if (e != d) { /* process the paired divisor */
                if (e > 2 && n % (e - 2) == 0) {
                    sum += (seq_t)e;
                } else if (n % (e + 2) == 0) {
                    sum += (seq_t)e;
                }
            }
        }
    }
    return sum;
}
