/*
 * A261681  a(n) = 2^n + binomial(n, floor(n/2)) - 1.
 * https://oeis.org/A261681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + binomial(n, floor(n/2)) - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t power = pow2(n);
    seq_t bin    = binomial(n, n / 2);
    return power + bin - (seq_t)1;
}
