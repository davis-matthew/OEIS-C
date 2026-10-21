/*
 * A290732  Number of distinct values of X*(3*X-1)/2 mod n.
 * https://oeis.org/A290732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = number of distinct values of X*(3*X-1)/2 mod n.
 *            Computation uses the multiplicative property: - for p = 2 or 3:
 *            a(p^e) = p^e - for p ≥ 5: a(p^e) = 1 + floor(p^(e+1)/(2p+2))
 * Range:     u8 n <= 16, u16 n <= 4412, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* inclusion of the big‑integer utility for raising integers to powers
   (required on all builds) */
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case */
    if (n == 1)
        return (seq_t)1;

    idx_t temp = n;
    seq_t result = (seq_t)1;
    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p != 0)
            continue;

        /* Count the exponent of prime p */
        idx_t e = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++e;
        }

        /* Contribution of the prime power p^e */
        seq_t contrib;
        if (p == 2 || p == 3)
            contrib = pow_uint(p, e);
        else
        {
            /* p >= 5 */
            seq_t denom = (seq_t)(2 * p + 2);
            seq_t term  = pow_uint(p, e + 1) / denom;
            contrib = term + (seq_t)1;
        }

        result = result * contrib;
    }

    /* Remaining factor > sqrt(temp) is a prime */
    if (temp > 1)
    {
        idx_t p = temp;             /* prime factor */
        idx_t e = 1;                /* exponent is 1 */

        seq_t contrib;
        if (p == 2 || p == 3)
            contrib = pow_uint(p, e);
        else
        {
            seq_t denom = (seq_t)(2 * p + 2);
            seq_t term  = pow_uint(p, e + 1) / denom;
            contrib = term + (seq_t)1;
        }

        result = result * contrib;
    }

    return result;
}
