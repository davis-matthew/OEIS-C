/*
 * A323703  Number of values of (X^3 + X) mod prime(n).
 * https://oeis.org/A323703
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) - 2*floor(prime(n)/6 + 1/2) for n >= 3; a(1) =
 *            1, a(2) = 3
 * Range:     u8 n <= 54, u16 n <= 6600, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)3;
    /* n >= 3 */
    uint32_t p = get_prime(n);          /* nth prime (fits in 32 bits for n <= 10000) */
    seq_t p_t = (seq_t)p;               /* promote to seq_t */
    seq_t k = (p_t + 3) / 6;             /* floor((p+3)/6) = floor(p/6 + 1/2) */
    return p_t - 2 * k;
}
