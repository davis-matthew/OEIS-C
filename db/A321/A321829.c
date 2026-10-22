/*
 * A321829  a(n) = Sum_{d|n, n/d==1 mod 4} d^5 - Sum_{d|n, n/d==3 mod 4} d^5.
 * https://oeis.org/A321829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n, n/d==1 mod 4} d^5 - Sum_{d|n, n/d==3 mod 4} d^5
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            /* divisor pair (i, n/i) */
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* process d1 */
            seq_t d1_pow5 = big_pow((seq_t)d1, 5);
            idx_t m1 = d2; /* n/d1 == d2 */
            if (m1 % 4 == 1) {
                sum += d1_pow5;
            } else if (m1 % 4 == 3) {
                sum -= d1_pow5;
            }

            /* if not a square, process d2 */
            if (d1 != d2) {
                seq_t d2_pow5 = big_pow((seq_t)d2, 5);
                idx_t m2 = d1; /* n/d2 == d1 */
                if (m2 % 4 == 1) {
                    sum += d2_pow5;
                } else if (m2 % 4 == 3) {
                    sum -= d2_pow5;
                }
            }
        }
    }
    return sum;
}
