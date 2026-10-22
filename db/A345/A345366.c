/*
 * A345366  a(n) = (p*q+1) mod (p+q) where p=prime(n) and q=prime(n+1).
 * https://oeis.org/A345366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (prime(n)*prime(n+1)+1) mod (prime(n)+prime(n+1))
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    seq_t p = (seq_t)get_prime(n);
    seq_t q = (seq_t)get_prime(n + 1);
    seq_t num = p * q + 1;
    seq_t denom = p + q;
    return num % denom;
}
