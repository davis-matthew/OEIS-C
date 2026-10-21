/*
 * A269327  a(n) = 7^prime(n).
 * https://oeis.org/A269327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^prime(n) (OEIS A269327)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Prime index is 1-based; prime(n) returns the nth prime as uint32_t */
    uint32_t prime = get_prime(n);
    /* Compute 7^prime using arbitrary-precision exponentiation */
    seq_t result = big_pow((seq_t)7, (seq_t)prime);
    return result;
}
