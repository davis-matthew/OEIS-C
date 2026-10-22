/*
 * A370296  Inverse Moebius transform of A322327.
 * https://oeis.org/A370296
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult, changed
 * Formula:   a(n) = ∏_p p^e∥n (1 + e + e²) (a multiplicative function)
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)   /* undefined for 0, but return 0 for safety */
        return (seq_t)0;

    idx_t tmp = n;              /* factorization of n */
    seq_t result = (seq_t)1;    /* product of terms */

    /* trial division: iterate over all integers up to √tmp */
    for (idx_t p = 2; p <= tmp / p; ++p)
    {
        if (tmp % p == 0)        /* p divides n */
        {
            idx_t e = 0;         /* exponent of prime p */
            while (tmp % p == 0)
            {
                ++e;
                tmp /= p;
            }
            /* term for p^e : 1 + e + e² */
            seq_t term = (seq_t)1 + (seq_t)e + (seq_t)e * (seq_t)e;
            result *= term;
        }
    }

    /* remaining prime factor (if any), exponent 1 */
    if (tmp > 1)
        result *= (seq_t)3;      /* 1 + 1 + 1² = 3 */

    return result;
}
