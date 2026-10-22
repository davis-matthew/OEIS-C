/*
 * A374431  Row product of A374433.
 * https://oeis.org/A374431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{p | n} p^{⌊n/p⌋}
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 29, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0 or 1, return 1.  The product is taken over all distinct prime
   factors p of n.  The exponent of p is the integer floor(n/p).  This
   produces the OEIS sequence A374431.  The implementation factors n by
   trial division and uses the provided big_pow utility for exponentiation. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n <= 1)            /* a(0) = a(1) = 1 */
        return (seq_t)1;

    idx_t orig = n;        /* remember the original n for exponent calculations */
    seq_t result = (seq_t)1;
    idx_t temp = n;

    /* factor n and compute the contribution of each distinct prime factor */
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            seq_t factor = big_pow((seq_t)p, orig / p);
            result = result * factor;
            while (temp % p == 0)       /* remove all occurrences of p */
                temp /= p;
        }
    }

    /* if a prime factor larger than sqrt(n) remains, it is the last one */
    if (temp > 1)
    {
        seq_t factor = big_pow((seq_t)temp, orig / temp);
        result = result * factor;
    }

    return result;
}
