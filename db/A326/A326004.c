/*
 * A326004  G.f.: Sum_{n>=0} (n+1)*(n+2)*(n+3)/3! * x^n * (1 + x^n)^n.
 * https://oeis.org/A326004
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*

Formula (OEIS A326004):
   a(0) = 1
   for n > 0,
   a(n) = Σ_{d | n,  n/d – 1 ≤ d}  binomial(d+3, 3) * binomial(d, n/d – 1)

This follows from the generating‑function description
   Σ_{n≥0} binomial(n+3,3) x^n (1+x^n)^n
which expands to the divisor sum above.
The implementation iterates over all divisors of n by scanning i up to √n
and processes both i and n/i as possible divisors.
The arithmetic uses the provided binomial function, and all
multiplications are performed in type seq_t to preserve arbitrary‑precision
behaviour when required.

*/

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* a(0) is defined as 1 */
        return (seq_t)1;

    seq_t sum = 0;

    /* Scan divisors i up to √n */
    for (idx_t i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            /* First divisor d = i */
            idx_t d = i;
            idx_t k = n / d - 1;          /* k = n/d – 1 */
            if (k <= d)
            {
                seq_t term = binomial(d + 3, 3);
                term *= binomial(d, k);
                sum += term;
            }

            /* Second divisor d = n/i, distinct from i */
            if (i != n / i)
            {
                d = n / i;
                k = n / d - 1;            /* k = i – 1 */
                if (k <= d)
                {
                    seq_t term = binomial(d + 3, 3);
                    term *= binomial(d, k);
                    sum += term;
                }
            }
        }
    }
    return sum;
}
