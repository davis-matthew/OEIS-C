/*
 * A321808  a(n) = Sum_{d divides n} (-1)^(d + n/d) * d^11.
 * https://oeis.org/A321808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = Sum_{d|n} (-1)^(d + n/d) * d^11
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 7, i64 n <= 52, i128 n <= 2989,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Sequence element type and index type are defined by the build. */

static seq_t pow11(idx_t base)
{
    seq_t r = 1;
    for (int i = 0; i < 11; ++i)
        r *= (seq_t)base;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* offset is 1, but guard against 0 */

    seq_t total = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            idx_t other = n / d;
            seq_t p_d = pow11(d);
            if (((d + other) & 1ULL) == 0)
                total += p_d;
            else
                total -= p_d;

            if (other != d)
            {
                seq_t p_o = pow11(other);
                if (((d + other) & 1ULL) == 0)   /* same parity as above */
                    total += p_o;
                else
                    total -= p_o;
            }
        }
    }
    return total;
}
