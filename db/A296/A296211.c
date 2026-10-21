/*
 * A296211  a(n) = 1 if sigma(n)-1 is a prime, 0 otherwise.
 * https://oeis.org/A296211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if sigma(n)-1 is a prime, 0 otherwise
 * Range:     u8 n <= 107, u16 n <= 9933, u32 n <= 9933, u64 n <= 9933,
 *            u128 n <= 9933, big any n
 * Helpers:   is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    seq_t s = sigma_of(n);
    seq_t diff = s - 1;
    return is_prime_big(diff) ? (seq_t)1 : (seq_t)0;
}
