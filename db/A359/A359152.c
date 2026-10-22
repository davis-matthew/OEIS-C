/*
 * A359152  a(n) = 1 if n is a number of the form 4u+3 with an odd number of
 *          prime factors (counted with multiplicity), otherwise 0.
 * https://oeis.org/A359152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n ≡ 3 (mod 4) and Ω(n) is odd; otherwise 0
 * Range:     u8 n <= 9620, u16 n <= 9620, u32 n <= 9620, u64 n <= 9620,
 *            u128 n <= 9620, big any n
 * Helpers:   count_prime_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/count_prime_factors.h"

seq_t a(idx_t n)
{
    if ((n & 3u) != 3u)
        return 0;
    seq_t omega = count_prime_factors((seq_t)n);
    if ((omega & 1u) != 0)
        return 1;
    return 0;
}
