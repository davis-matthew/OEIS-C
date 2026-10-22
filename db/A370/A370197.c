/*
 * A370197  a(n) is the number of ways to place n indistinguishable balls into
 *          n distinguishable boxes with at least 4 boxes remaining empty.
 * https://oeis.org/A370197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n-1,n) - binomial(n,2)*binomial(n-1,2) * -
 *            binomial(n,3)*binomial(n-1,3) - n*(n-1) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t result = binomial(2*n - 1, n);

    if (n >= 2) {
        seq_t b2 = binomial(n, 2);
        seq_t b3 = binomial(n - 1, 2);
        result -= b2 * b3;
    }

    if (n >= 3) {
        seq_t c2 = binomial(n, 3);
        seq_t c3 = binomial(n - 1, 3);
        result -= c2 * c3;
    }

    seq_t prod = (seq_t)n * ((seq_t)n - 1);
    result -= prod;

    result -= 1;
    return result;
}
