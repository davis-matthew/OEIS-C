/*
 * A360428  Inverse Mobius transformation of A338164.
 * https://oeis.org/A360428
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = product_{p^e||n} ((e+1)*p^(2e) - e*p^(2e-2))
 *            (multiplicative)
 * Range:     u8 n <= 9, u16 n <= 83, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_seq_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq_val.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but harmless */
    seq_t result = 1;
    idx_t m = n;

    for (idx_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            idx_t e = 0;
            while (m % p == 0) { m /= p; ++e; }

            seq_t pow2e = pow_seq_val((seq_t)p, 2 * e);
            seq_t pow2e_minus2 = (e >= 2) ?
                                 pow_seq_val((seq_t)p, 2 * e - 2) :
                                 1;                /* p^0 */

            seq_t term = ((seq_t)(e + 1)) * pow2e - (seq_t)e * pow2e_minus2;
            result *= term;
        }
    }

    /* Remaining prime factor with exponent 1 (if any) */
    if (m > 1)
    {
        seq_t pow2e = pow_seq_val((seq_t)m, 2);  /* p^2 */
        seq_t term  = ((seq_t)2) * pow2e - (seq_t)1; /* 2*p^2 - 1 */
        result *= term;
    }

    return result;
}
