/*
 * A378988  a(n) = 2*n XOR 1+sigma(n), where XOR is bitwise-xor, A003987.
 * https://oeis.org/A378988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (2*n) XOR (1 + sigma(n))
 * Range:     u8 n <= 107, u16 n <= 9917, u32 n <= 9917, u64 n <= 9917,
 *            u128 n <= 9917, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

seq_t a(idx_t n)
{
    seq_t two_n  = (seq_t)n * 2;
    seq_t sigma1 = sigma_of(n) + 1;
    return two_n ^ sigma1;
}
