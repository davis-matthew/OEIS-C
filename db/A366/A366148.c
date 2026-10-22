/*
 * A366148  The sum of divisors of the cubefree part of n (A360539).
 * https://oeis.org/A366148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} a(p^e) = (p^(e+1)-1)/(p-1) if e ≤ 2, a(p^e) = 1
 *            if e > 2
 * Range:     u8 n <= 125, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is multiplicative; for e = 1 the factor is (p+1),
   for e = 2 the factor is p² + p + 1.  No prime with exponent >2
   contributes to the product.  The implementation uses simple
   trial‑division which is fast enough for n up to at least 10 000. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* The types seq_t and idx_t are supplied by the build environment. */

seq_t a(idx_t n)
{
    seq_t result = 1;           /* Multiplicative identity     */
    idx_t temp   = n;           /* Remaining part to factorise */

    for (idx_t p = 2; p <= temp / p; ++p)    /* trial‑division          */
    {
        if (temp % p == 0)
        {
            unsigned e = 0;               /* exponent of prime p         */
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            if (e == 1)                /* factor (p + 1)              */
            {
                result *= ((seq_t)p + 1);
            }
            else if (e == 2)           /* factor (p² + p + 1)         */
            {
                seq_t pp = (seq_t)p * (seq_t)p;  /* p² */
                result *= (pp + (seq_t)p + 1);
            }
            /* e > 2 contributes 1 – nothing to multiply          */
        }
    }

    if (temp > 1)                    /* remaining prime exponent 1  */
    {
        result *= ((seq_t)temp + 1);
    }

    return result;
}
