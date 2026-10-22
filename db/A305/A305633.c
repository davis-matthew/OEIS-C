/*
 * A305633  Expansion of Sum_{r not a perfect power} x^r/(1 + x^r).
 * https://oeis.org/A305633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = sum_{r|n, r not a perfect power} (-1)^{n/r-1} (with the
 *            convention that 1 is counted as a perfect power) a(0) = 0
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>


static bool is_perfect_power(idx_t x)
{
    if (x < 2)                       /* 1 is treated as a perfect power */
        return true;

    /* For e from 2 up to log2(x) */
    for (int e = 2; ; ++e)
    {
        /* if 2^e > x, no larger e can satisfy base^e == x */
        unsigned long long pow2 = 1;
        for (int i = 0; i < e; ++i)
            pow2 <<= 1;               /* multiply by 2 */
        if (pow2 > x)
            break;

        idx_t base = 2;
        for (;; ++base)
        {
            unsigned long long prod = 1;
            int i;
            for (i = 0; i < e; ++i)
            {
                prod *= base;
                if (prod > x)
                    break;
            }
            if (i == e && prod == x)
                return true;           /* exact perfect power found */
            if (prod > x)
                break;                 /* further bases will only increase */
        }
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* defined by the OEIS offset */

    seq_t sum = 0;

    /* iterate over all divisors of n */
    for (idx_t d = 1; (unsigned long long)d * d <= n; ++d)
    {
        if (n % d != 0)
            continue;

        idx_t r1 = d;
        idx_t r2 = n / d;

        if (!is_perfect_power(r1))
        {
            seq_t sign = ((n / r1) & 1ULL) ? 1 : -1;   /* (n/r1)%2 == 1 ? +1 : -1 */
            sum += sign;
        }

        if (r2 != r1 && !is_perfect_power(r2))
        {
            seq_t sign = ((n / r2) & 1ULL) ? 1 : -1;
            sum += sign;
        }
    }

    return sum;
}
