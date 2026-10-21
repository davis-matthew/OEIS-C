/*
 * A287932  a(n) = least k > n such that lpf(n) = lpf(k), where lpf = least
 *          prime factor (A020639).
 * https://oeis.org/A287932
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k > n such that lpf(n) = lpf(k), where lpf * is the
 *            least prime factor. The scheme uses quick shortcuts for * primes
 *            and even numbers, and a linear search otherwise
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, lpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
/* Provided utilities */
#include "../utils/lpf.h"     /* lpf(idx_t n) */
#include "../utils/is_prime.h" /* is_prime(idx_t n) – optional, not used */

seq_t a(idx_t n)
{
    /* lpf returns the least prime factor of n (for primes it returns n),
       so we can detect primes simply by comparing with n. */
    seq_t p = (seq_t)lpf(n);

    /* Prime case: a(p) = p^2 */
    if (p == (seq_t)n)
        return (seq_t)n * (seq_t)n;

    /* Even case for any n > 2: a(2k) = 2k + 2 */
    if ((n & 1) == 0)
        return (seq_t)n + (seq_t)2;

    /* Odd composite case: search for the next number with the same lpf */
    idx_t k = n + 1;
    seq_t lpf_k;
    for (;; ++k)
    {
        lpf_k = (seq_t)lpf(k);
        if (lpf_k == p)
            return (seq_t)k;
    }
}
