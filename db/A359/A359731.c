/*
 * A359731  a(n) = (1/2) * Sum_{d|n} (2*d)^d.
 * https://oeis.org/A359731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/2) * Sum_{d|n} (2*d)^d
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* behavior for n < 1 is undefined by OEIS */
    seq_t sum = 0;

    idx_t i;
    for (i = 1; i <= n / i; ++i)   /* iterate over divisors efficiently */
    {
        if (n % i == 0) {
            /* first divisor d = i */
            seq_t base1 = (seq_t)2 * (seq_t)i;
            sum += seq_pow(base1, i);

            /* second divisor d = n / i, if distinct */
            idx_t d2 = n / i;
            if (d2 != i) {
                seq_t base2 = (seq_t)2 * (seq_t)d2;
                sum += seq_pow(base2, d2);
            }
        }
    }

    sum /= 2;   /* division is exact because all terms are even */
    return sum;
}
