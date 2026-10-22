/*
 * A321543  a(n) = Sum_{d|n} (-1)^(d-1)*d^2.
 * https://oeis.org/A321543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d-1) * d^2 Sequence A321543 (OEIS). The
 *            sequence is multiplicative. For a prime power p^e: a(2^e) = 2 -
 *            (2^(2e+2) - 1) / 3 a(p^e) = (p^(2e+2) - 1) / (p^2 - 1) for p > 2
 *            The function uses seq_pow from the utils library
 * Range:     i8 n <= 4, i16 n <= 16, i32 n <= 1802, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1; n == 0 is undefined */
    seq_t result = (seq_t)1;
    idx_t temp = n;

    for (idx_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            /* count exponent e of prime p */
            idx_t e = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++e;
            }

            seq_t factor;
            if (p == (idx_t)2)
            {
                /* a(2^e) = 2 - (2^(2e+2) - 1) / 3 */
                seq_t pow_val = seq_pow((seq_t)2, (idx_t)(2 * e + 2));
                seq_t num = pow_val - (seq_t)1;
                factor = (seq_t)2 - num / (seq_t)3;
            }
            else
            {
                /* a(p^e) = (p^(2e+2) - 1) / (p^2 - 1) */
                seq_t pow_val = seq_pow((seq_t)p, (idx_t)(2 * e + 2));
                seq_t num = pow_val - (seq_t)1;
                seq_t denom = (seq_t)p * (seq_t)p - (seq_t)1;
                factor = num / denom;
            }

            result *= factor;
        }
    }

    /* remaining prime factor > 1 (exponent 1) */
    if (temp > 1)
    {
        idx_t p = temp;
        seq_t factor;
        if (p == (idx_t)2)
        {
            /* exponent e = 1 for the remaining factor 2 */
            seq_t pow_val = seq_pow((seq_t)2, (idx_t)4);  /* 2^(2*1+2) */
            seq_t num = pow_val - (seq_t)1;
            factor = (seq_t)2 - num / (seq_t)3;
        }
        else
        {
            seq_t pow_val = seq_pow((seq_t)p, (idx_t)4);  /* p^(2*1+2) */
            seq_t num = pow_val - (seq_t)1;
            seq_t denom = (seq_t)p * (seq_t)p - (seq_t)1;
            factor = num / denom;
        }

        result *= factor;
    }

    return result;
}
