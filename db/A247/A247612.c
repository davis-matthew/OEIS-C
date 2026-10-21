/*
 * A247612  a(n) = Sum_{k=0..7} binomial(14,k)*binomial(n,k).
 * https://oeis.org/A247612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..7} binomial(14,k) * binomial(n,k)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 599, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Precomputed binomial(14,k) for k = 0..7 */
    static const seq_t C[8] = {
        1,  14,  91,  364, 1001, 2002, 3003, 3432
    };

    seq_t sum = 0;
    unsigned int k;
    for (k = 0; k <= 7; ++k)
        sum += C[k] * binomial(n, k);

    return sum;
}
