/*
 * A352050  Sum of the 4th powers of the divisor complements of the odd proper
 *          divisors of n.
 * https://oeis.org/A352050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 * Sum_{d|n, d<n, d odd} 1/d^4 Equivalent: a(n) =
 *            Sum_{odd div. d of n, d<n} (n/d)^4 (All arithmetic is exact
 *            integer, no floating point.)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {                         /* d is a divisor of n          */
            /* contribution of d itself */
            if ((d & 1) && d < n) {               /* d is odd and proper          */
                idx_t t = n / d;                  /* t = n/d, integer             */
                seq_t t4 = (seq_t)t;
                t4 *= t; t4 *= t; t4 *= t;         /* (n/d)^4                       */
                sum += t4;
            }
            /* contribution of the complementary divisor e = n/d */
            idx_t e = n / d;
            if (e != d && (e & 1) && e < n) {      /* e odd, distinct, proper      */
                seq_t d4 = (seq_t)d;
                d4 *= d; d4 *= d; d4 *= d;         /* d^4 = (n/e)^4                 */
                sum += d4;
            }
        }
    }
    return sum;
}
