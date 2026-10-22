/*
 * A340649  a(n) = (n*prime(n+1)) mod prime(n).
 * https://oeis.org/A340649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = (n * prime(n+1)) mod prime(n)
 * Range:     u8 n <= 53, u16 n <= 6558, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* a(1) = 1, a(2) = 1, a(3) = 1, ... */
    seq_t prime_n  = get_prime(n);        /* prime(n)   */
    seq_t prime_np = get_prime(n + 1);    /* prime(n+1) */
    return ((seq_t)n * prime_np) % prime_n;
}
