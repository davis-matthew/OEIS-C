/*
 * A306842  a(n) = Sum_{d|n} binomial(n,d)*binomial(n,n/d).
 * https://oeis.org/A306842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} binomial(n,d)*binomial(n,n/d)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 55, u128 n <= 119,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            idx_t e = n / d;
            seq_t b1 = binomial(n, d);
            seq_t b2 = binomial(n, e);
            seq_t prod = b1 * b2;
            if (d == e) {
                sum += prod;
            } else {
                sum += prod * ((seq_t)2);
            }
        }
    }
    return sum;
}
