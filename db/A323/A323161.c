/*
 * A323161  Lexicographically earliest sequence such that a(i) = a(j) => f(i)
 *          = f(j) for all i, j, where f(n<=3) = -n, f(n) = 0 if n-1 is an odd
 *          prime, and f(n) = floor((n-1)/2) for all other numbers.
 * https://oeis.org/A323161
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = * a(1)=1, a(2)=2, a(3)=3; * For n>3, let m=n-1. * If m is
 *            an odd prime, a(n)=4; * else a(n)= floor(m/2)+3
 * Range:     u8 n <= 506, u16 n <= 8617, u32 n <= 8617, u64 n <= 8617,
 *            u128 n <= 8617, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This follows from a(i)=1+A322809(i‑1) with the definition of f.  */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the offset for n = 1 .. 3 directly. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)3;

    idx_t m = n - 1;               /* m >= 3 */
    /* Check for odd prime. */
    if ( (m & 1u) && is_prime(m) )
        return (seq_t)4;           /* f(n)=0 -> a(n)=4 */

    /* f(n) = floor((n-1)/2) for all other cases (n > 3). */
    idx_t f = m >> 1;              /* integer division by 2 */
    /* For f >= 2 the sequence uses a(n)=f+3. */
    return (seq_t)f + (seq_t)3;
}
