/*
 * A338547  a(n) = n^2 * Sum_{d|n} (-1)^(n/d + 1) * mu(d) / d^2.
 * https://oeis.org/A338547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = n^2 * Sum_{d|n} (-1)^(n/d+1) * mu(d)/d^2
 * Range:     i8 n <= 12, i16 n <= 165, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function uses the multiplicative formula: */
/*   a(p^e) = -5              if p = 2 and e = 1 */
/*   a(2^e) = -3 * 2^(2*(e-1)) if p = 2 and e > 1 */
/*   a(p^e) = (p^2-1) * p^(2*(e-1))          if p > 2          */
#include <stdint.h>

/* fast exponentiation for seq_t (works for small exponents on the given limits) */
static seq_t pow_seq(seq_t base, unsigned exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1u)
            result *= base;
        base *= base;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* a(1) = 1 */
    if (n == 1)
        return 1;

    uint64_t temp = (uint64_t)n;
    seq_t res = 1;

    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p != 0)
            continue;

        unsigned exp = 0;
        while (temp % p == 0)
        {
            temp /= p;
            ++exp;
        }

        seq_t contrib;
        if (p == 2)
        {
            if (exp == 1)
                contrib = -5;
            else
                contrib = -3 * pow_seq((seq_t)2, 2 * (exp - 1));
        }
        else
        {
            contrib = ((seq_t)p * (seq_t)p - 1) * pow_seq((seq_t)p, 2 * (exp - 1));
        }

        res *= contrib;
    }

    /* remaining prime factor, if any */
    if (temp > 1)
    {
        uint64_t p = temp;
        /* exponent is 1 */
        seq_t contrib;
        if (p == 2)
            contrib = -5; /* e == 1 case */
        else
            contrib = ((seq_t)p * (seq_t)p - 1); /* p^(0) = 1 */

        res *= contrib;
    }

    return res;
}
