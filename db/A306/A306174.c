/*
 * A306174  a(n) = (prime(n)^6 - 1)/504.
 * https://oeis.org/A306174
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n)^6 - 1)/504 (offset 5)
 * Range:     u8 none, u16 none, u32 n <= 12, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   big_pow, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    /* For n below the offset we simply return 0 (no defined value). */
    if (n < 5) return (seq_t)0;

    /* Retrieve the n-th prime.  The helper returns a uint32_t. */
    uint32_t p = get_prime(n);
    seq_t prime = (seq_t)p;

    /* Compute prime^6 using the big_pow helper, which works for any seq_t. */
    seq_t pow6 = big_pow(prime, (seq_t)6);

    /* Subtract 1 and divide by 504.  All constants are cast to seq_t. */
    seq_t result = (pow6 - (seq_t)1) / (seq_t)504;

    return result;
}
