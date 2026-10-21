/*
 * A259175  a(n) = 1 if n prime, otherwise prime(n).
 * https://oeis.org/A259175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is prime; otherwise a(n) = prime(n)
 * Range:     u8 n <= 54, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   get_prime, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (is_prime(n))
        return (seq_t)1;
    else
        return (seq_t)get_prime(n);
}
