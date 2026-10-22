/*
 * A342432  a(n) = Sum_{k=1..n} gcd(k,n)^(n-2).
 * https://oeis.org/A342432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} phi(n/d) * d^(n-2)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   big_pow, phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* The formula is valid for n >= 2.  For n == 1 the definition
       that uses gcd(k,n)^(n-2) collapses to 1, so we special‑case it. */
    if (n == 1)
        return (seq_t)1;

    seq_t sum = (seq_t)0;
    /* exponent (n-2) fits in seq_t because n is uint64_t. */
    seq_t exp = (seq_t)(n - 2);

    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            idx_t d2 = n / d;

            /* term for divisor d */
            seq_t term1 = phi(d2);                     /* φ(n/d)  */
            seq_t pow1  = big_pow((seq_t)d, exp);      /* d^(n-2) */
            sum += term1 * pow1;

            if (d != d2)
            {
                /* term for divisor d2 (complementary divisor) */
                seq_t term2 = phi(d);                 /* φ(n/d2) = φ(d) */
                seq_t pow2  = big_pow((seq_t)d2, exp); /* d2^(n-2) */
                sum += term2 * pow2;
            }
        }
    }
    return sum;
}
