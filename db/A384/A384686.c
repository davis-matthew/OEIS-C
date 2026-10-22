/*
 * A384686  a(n) = 2^(n-4)*(5*binomial(n,5) + 6*binomial(n,4)).
 * https://oeis.org/A384686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-4) * (5*binomial(n,5) + 6*binomial(n,4))
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 45, u128 n <= 103,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* For n < 4 the sequence is defined to be 0. */
    if (n < 4)
        return 0;

    /* Compute 5*binomial(n,5) and 6*binomial(n,4). */
    seq_t term5 = binomial(n, 5);
    term5 *= 5;

    seq_t term4 = binomial(n, 4);
    term4 *= 6;

    /* Sum the terms and multiply by 2^(n-4). */
    seq_t sum = term5 + term4;
    seq_t pow = pow2(n - 4);
    return pow * sum;
}
