/*
 * A278741  Odd numbers k such that tau(k-1) is a prime.
 * https://oeis.org/A278741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(prime(n) - 1) + 1, where prime(n) is the n‑th prime.
 *            This implements OEIS A278741: the smallest odd number k such
 *            that tau(k-1) equals the n‑th prime
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   get_prime, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* offset = 1; behaviour for n < 1 is undefined */
    if (n == 0U)
        return (seq_t)0U;

    /* exponent = prime(n) - 1  */
    idx_t exp = (idx_t) get_prime(n) - 1U;

    /* k = 2^exp + 1 */
    return pow2(exp) + (seq_t)1U;
}
