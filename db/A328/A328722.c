/*
 * A328722  Dirichlet g.f.: 1 / zeta(s-1)^2.
 * https://oeis.org/A328722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, mult
 * Formula:   not stated
 * Range:     i8 n <= 29, i16 n <= 2089, i32 n <= 8230, i64 n <= 8230,
 *            i128 n <= 8230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328722: Dirichlet inverse of A038040.
   Formula: a(n) = n * A007427(n)
   where A007427 is a multiplicative function defined by
   A007427(p)   = -2   for prime p,
   A007427(p²)  =  1   for prime squares,
   A007427(pᵉ)  =  0   for exponents e≥3.
   Initial values:
   a(1)=1, a(2)=-4, a(3)=-6, a(4)=4, a(5)=-10, ... */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, but return 0 to avoid UB */
        return 0;

    idx_t temp = n;
    seq_t prod = 1;   /* product of A007427 prime‑power factors */

    for (idx_t p = 2; (seq_t)p * (seq_t)p <= (seq_t)temp; ++p)
    {
        if (temp % p == 0)
        {
            int e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            if (e == 1)
                prod *= (seq_t)(-2);   /* A007427(p)   = -2 */
            else if (e == 2)
                prod *= (seq_t)(1);    /* A007427(p²) =  1  */
            else            /* e >= 3 => factor 0 */
            {
                prod = 0;
                break;
            }
        }
    }

    /* Remaining prime factor, if any */
    if (prod != 0 && temp > 1)
    {
        /* temp is a prime with exponent 1 */
        prod *= (seq_t)(-2);
    }

    /* a(n) = n * prod */
    return (seq_t)n * prod;
}
