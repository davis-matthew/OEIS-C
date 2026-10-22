/*
 * A318468  a(n) = 2*n AND A000203(n), where AND is bitwise-and (A004198) and
 *          A000203 = sum of divisors.
 * https://oeis.org/A318468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (2*n) AND σ(n) where σ(n) is the sum of divisors of n
 * Range:     u8 n <= 131, u16 n <= 9682, u32 n <= 9682, u64 n <= 9682,
 *            u128 n <= 9682, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t sigma = sigma_of(n);
    seq_t two_n = (seq_t)n << 1;
    return two_n & sigma;
}
