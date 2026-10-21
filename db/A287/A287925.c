/*
 * A287925  a(n) = prime(1)^4 + prime(n)^4.
 * https://oeis.org/A287925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 + prime(n)^4
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = prime(1)^4 + prime(n)^4 with prime(1)=2. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (prime(1) = 2). */
    uint32_t p = get_prime(n);

    /* Compute p^4 using seq_t arithmetic to avoid overflow. */
    seq_t val = (seq_t)p;   /* p */
    val *= val;             /* p^2 */
    val *= val;             /* p^4 */
    val += (seq_t)16;       /* plus 2^4 */

    return val;
}
