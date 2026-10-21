/*
 * A280363  a(n) = floor(log_p(n)) where p = A020639(n), i.e., the least prime
 *          factor of n.
 * https://oeis.org/A280363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log_{p(n)} n), where p(n) is the least * prime
 *            factor of n. For n = 1 the value is 0 by definition
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _least_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_least_prime_factor.h"

seq_t a(idx_t n)
{
    /* a(1) = 0 by definition */
    if (n <= 1u)
        return (seq_t)0;

    /* compute least prime factor of n */
    uint64_t p = _least_prime_factor((uint64_t)n);

    /* find the largest integer e with p^e <= n */
    seq_t val = 1;   /* current power of p */
    seq_t e   = 0;   /* exponent counter */

    /* loop using only integer arithmetic; the comparison converts
     * `val` to idx_t to avoid issues with seq_t being larger than
     * the machine integer type.  Because `val` never exceeds `n`,
     * the cast is safe. */
    while ((idx_t)val <= (idx_t)n / p)
    {
        val *= p;
        ++e;
    }

    return e;
}
