/*
 * A359796  a(n) = Sum_{d|n} (2*d)^(d-1).
 * https://oeis.org/A359796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} (2*d)^(d-1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    // iterate through divisors d of n
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            // first divisor d = i
            idx_t d1 = i;
            seq_t base1 = (seq_t)2 * (seq_t)d1;
            seq_t exp1  = (seq_t)(d1 - 1);
            seq_t term1 = big_pow(base1, exp1);
            sum += term1;

            // second divisor d = n / i, if different
            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t base2 = (seq_t)2 * (seq_t)d2;
                seq_t exp2  = (seq_t)(d2 - 1);
                seq_t term2 = big_pow(base2, exp2);
                sum += term2;
            }
        }
    }
    return sum;
}
