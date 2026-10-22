/*
 * A380301  Semiperimeter of the unique primitive Pythagorean triple whose
 *          inradius is the n-th odd prime and whose short leg is an even
 *          number.
 * https://oeis.org/A380301
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (prime_{n+2})^2 + 3*prime_{n+2} + 2 * where prime_{k} is
 *            the k-th prime (prime_1 = 2)
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* the n-th odd prime is the (n+2)-nd prime because
       prime_1 = 2, prime_2 = 3, prime_3 = 5, ...            */
    uint64_t p_u64 = get_prime(n + 2);   /* p < 2^32 for n up to at least 10^8 */
    seq_t p = (seq_t)p_u64;

    seq_t res = p * p + (seq_t)3 * p + (seq_t)2;
    return res;
}
