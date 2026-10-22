/*
 * A384556  The sum of the exponential divisors of n that are cubefree.
 * https://oeis.org/A384556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = product over prime powers p^e in n of ( p if e is odd ) (
 *            p + p² if e is even ) with a(1)=1. The product excludes the
 *            divisor 1 for n>1
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* Trial division up to sqrt(temp).  For each found prime factor p,
       count its exponent e, compute the corresponding term, and
       multiply it into the running product. */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }

            seq_t sp   = (seq_t)p;
            seq_t term = (exp & 1) ? sp : sp + sp * sp;
            result *= term;
        }
    }

    /* Any remaining factor is a prime > sqrt(original n). */
    if (temp > 1)
    {
        seq_t sp = (seq_t)temp;          /* exponent 1 (odd) */
        result *= sp;
    }

    return result;
}
