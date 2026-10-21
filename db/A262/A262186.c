/*
 * A262186  a(n) = prime(n)^3 - n^3.
 * https://oeis.org/A262186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^3 - n^3
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t
a(idx_t n)
{
    /* Convert the nth prime to seq_t and compute the difference */
    seq_t p  = (seq_t)get_prime(n);        /* prime(n) */
    seq_t p3 = p * p * p;                  /* prime(n)^3 */
    seq_t n3 = (seq_t)n * n * n;           /* n^3 */
    return p3 - n3;                        /* a(n) */
}
