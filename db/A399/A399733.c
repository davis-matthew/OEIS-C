/*
 * A399733  The sum of the unitary divisors d of n such that n/d is a powerful
 *          number (A001694).
 * https://oeis.org/A399733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n * A034448(n) / A363289(n) or equivalently: a is
 *            multiplicative with a(p) = p, a(p^e) = p^e + 1 for e ≥ 2 (OEIS
 *            A399733)
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Compute the desired sequence term for n ≥ 1.
   For n = 0 it returns 0 (undefined for the sequence). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* undefined at offset 0, safe fallback */

    /* 1 has no prime factors; the product of an empty set is 1 */
    if (n == 1)
        return 1;           /* a(1) = 1 */

    idx_t temp = n;
    seq_t result = 1;

    /* trial division up to √temp */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p != 0)
            continue;

        /* count exponent of p in temp */
        idx_t e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }

        seq_t term;
        if (e == 1)
            term = (seq_t)p;
        else
        {
            /* compute p^e */
            seq_t pow = 1;
            seq_t base = (seq_t)p;
            idx_t k;
            for (k = 0; k < e; ++k)
                pow *= base;
            term = pow + 1;
        }

        result *= term;
    }

    /* if any prime factor > √n remains, its exponent must be 1 */
    if (temp > 1)
        result *= (seq_t)temp;

    return result;
}
