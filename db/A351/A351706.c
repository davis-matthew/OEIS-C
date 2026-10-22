/*
 * A351706  For any nonnegative number n with binary expansion Sum_{k >= 0}
 *          b_k * 2^k, a(n) is the denominator of d(n) = Sum_{k >= 0} b_k *
 *          2^A130472(k). See A351705 for the numerators.
 * https://oeis.org/A351706
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, frac
 * Formula:   a(n) = 2^(-min_{k: bit k of n set} e_k) where e_k = -((k+1)/2)
 *            for odd k e_k = ((k/2) & 1) for even k This implements OEIS
 *            sequence A351706 (denominators of d(n))
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */
    int min_exp = 0;               /* minimal exponent among set bits */
    idx_t tmp = n;
    idx_t bitpos = 0;

    while (tmp)
    {
        if (tmp & 1)
        {
            int exp;
            if (bitpos & 1) /* odd index */
                exp = -((int)((bitpos + 1) / 2));
            else           /* even index */
                exp = ((int)(bitpos / 2)) & 1;

            if (exp < min_exp)
                min_exp = exp;
        }
        tmp >>= 1;
        ++bitpos;
    }

    if (min_exp <= 0)
        return (seq_t)1 << (-min_exp);
    else
        return (seq_t)1;
}
