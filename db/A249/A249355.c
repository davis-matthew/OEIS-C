/*
 * A249355  Remainder of n!+2 divided by n+2.
 * https://oeis.org/A249355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n! + 2) mod (n + 2)
 * Range:     u8 n <= 2048, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: use the fact that for n+2 > 4, a(n) = 3 if n+2 is prime, otherwise 2. Special cases: a(0)=1, a(1)=0, a(2)=0. */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n <= 2)
        return (seq_t)0;
    idx_t p = n + (idx_t)2;
    if (is_prime(p))
        return (seq_t)3;
    return (seq_t)2;
}
