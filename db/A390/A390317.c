/*
 * A390317  Multiplicative sequence a(n) with a(p^e) = 2 + (p^e + p^(e-1) - 2)
 *          * (p+1) / (p-1) for prime p and e > 0.
 * https://oeis.org/A390317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   Multiplicative: a(pⁿ) = 2 + (pⁿ + pⁿ⁻¹ – 2) * (p+1) / (p-1) for
 *            prime p and n>0 a(1) = 1
 * Range:     u8 n <= 35, u16 n <= 3359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* compute base^exp as seq_t */
static seq_t pow_uint(uint64_t base, idx_t exp)
{
    seq_t res = 1;
    seq_t b   = (seq_t)base;
    while (exp)
    {
        if (exp & 1U) res *= b;
        b *= b;
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t result = 1;
    idx_t temp   = n;

    /* factorise temp by trial division */
    for (idx_t p = 2; p <= temp / p; ++p)
    {
        if (temp % p == 0)
        {
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            seq_t pe  = pow_uint(p, e);        /* p^e              */
            seq_t pe1 = pow_uint(p, e - 1);    /* p^(e-1)          */

            /* a(p^e) = 2 + (pe + pe1 - 2) * (p+1) / (p-1) */
            seq_t term = ((pe + pe1 - (seq_t)2) * (seq_t)(p + 1))
                        / (seq_t)(p - 1) + (seq_t)2;
            result *= term;
        }
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        idx_t p = temp;
        seq_t pe  = (seq_t)p;     /* p¹  */
        seq_t pe1 = (seq_t)1;     /* p⁰  */

        seq_t term = ((pe + pe1 - (seq_t)2) * (seq_t)(p + 1))
                    / (seq_t)(p - 1) + (seq_t)2;
        result *= term;
    }

    return result;
}
