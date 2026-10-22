/*
 * A358336  Multiplicative sequence with a(p^e) = ((p-1) * (1 + e*(e+1)/2) +
 *          e) * p^(e-1) for prime p and e > 0.
 * https://oeis.org/A358336
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = multiplicative function with a(p^e) = ((p-1) * (1 +
 *            e*(e+1)/2) + e) * p^(e-1) for prime p, e > 0 a(1) = 1 (This
 *            follows the OEIS definition of A358336.)
 * Range:     u8 n <= 31, u16 n <= 1151, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;          /* base case */

    idx_t temp = n;
    seq_t result = (seq_t)1;

    /* trial division up to sqrt(temp) */
    for (idx_t p = (idx_t)2; p * p <= temp; ++p)
    {
        if (temp % p != (idx_t)0)
            continue;

        /* count exponent e of prime p */
        int e = 0;
        while (temp % p == (idx_t)0)
        {
            temp /= p;
            ++e;
        }

        /* compute (p-1)*(1 + e*(e+1)/2) + e */
        seq_t e_seq = (seq_t)e;
        seq_t term = (seq_t)1 + e_seq * (e_seq + (seq_t)1) / (seq_t)2;
        seq_t factor = (seq_t)(p - (idx_t)1) * term + e_seq;

        /* multiply by p^(e-1) */
        seq_t p_pow = (seq_t)1;
        for (int k = 1; k < e; ++k)
            p_pow *= (seq_t)p;

        factor *= p_pow;
        result *= factor;
    }

    /* if a prime factor larger than sqrt(n) remains */
    if (temp > (idx_t)1)
    {
        seq_t p_seq = (seq_t)temp;
        seq_t e_seq = (seq_t)1;
        seq_t term   = (seq_t)1 + e_seq * (e_seq + (seq_t)1) / (seq_t)2;  /* = 2 */
        seq_t factor = p_seq - (seq_t)1;                                 /* p-1 */
        factor *= term;                                                  /* (p-1)*2 */
        factor += e_seq;                                                 /* +1 */
        result *= factor;                                                /* final factor */
    }

    return result;
}
