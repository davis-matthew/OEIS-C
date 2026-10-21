/*
 * A284004  a(n) = A046523(A284003(n)).
 * https://oeis.org/A284004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A046523(A284003(n)) * A284003(n) is defined recursively
 *            by repeatedly setting * n = n-p + p/2 or n-p - p/2 where p is
 *            the largest power of 2 ≤ n. * A046523 is the primorial (product
 *            of the first k primes). * Thus a(n) is the product of the first
 *            k(n) primes
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 511, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   _a284003, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/get_prime.h"   /* provides uint32_t get_prime(idx_t); */
#include "../utils/_a284003.h"

seq_t a(idx_t n)
{
    idx_t k = _a284003(n);
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res *= (seq_t)get_prime(i);
    }
    return res;
}
