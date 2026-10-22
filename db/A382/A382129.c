/*
 * A382129  Fractalization of the prime numbers.
 * https://oeis.org/A382129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = a(n); a(2n-1) = prime(n) (where prime(n) is the nth
 *            prime)
 * Range:     u8 n <= 108, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* For n = 0 (out of range) return 0 */
    if (n == 0) return (seq_t)0;

    /* Strip all trailing zeros: find odd part m of n */
    idx_t m = n;
    while ((m & 1) == 0) {
        m >>= 1;
    }

    /* For odd m = 2k-1, we need the k-th prime */
    idx_t k = (m + 1) >> 1;
    return (seq_t) get_prime(k);
}
