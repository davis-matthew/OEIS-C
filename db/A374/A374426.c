/*
 * A374426  a(n) = n*(n + 1)/2 + pi(n), where pi(n) = A000720(n) is the prime
 *          counting function.
 * https://oeis.org/A374426
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 + pi(n)
 * Range:     u8 n <= 21, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/prime_pi.h"

seq_t a(idx_t n)
{
    seq_t triangular = (seq_t)n * ((seq_t)n + 1) / 2;
    return triangular + prime_pi(n);
}
