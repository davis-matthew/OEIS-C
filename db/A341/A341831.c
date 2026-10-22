/*
 * A341831  Dirichlet g.f.: 1 / zeta(s)^5.
 * https://oeis.org/A341831
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative with a(p^e) = (-1)^e * binomial(5, e);
 *            a(1) = 1. binomial(5, e) = 0 for e > 5
 * Range:     i8 n <= 59, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: binomial coefficient for n = 5.  Returns 0 for e > 5. */
static seq_t binom5(int e)
{
    switch (e)
    {
        case 0: return (seq_t)1;
        case 1: return (seq_t)5;
        case 2: return (seq_t)10;
        case 3: return (seq_t)10;
        case 4: return (seq_t)5;
        case 5: return (seq_t)1;
        default: return (seq_t)0;
    }
}

/* Sequence A341831 */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;     /* a(1) = 1 */

    seq_t result = (seq_t)1;          /* multiplicative identity */
    idx_t remaining = n;              /* work on a copy of n */

    for (idx_t p = 2; p <= remaining / p; ++p)
    {
        if (remaining % p == 0)
        {
            int exp = 0;
            while (remaining % p == 0)
            {
                remaining /= p;
                ++exp;
            }

            seq_t bin = binom5(exp);
            if (bin == 0)            /* exponent > 5 → factor 0 */
            {
                result = (seq_t)0;
                break;
            }

            seq_t factor = (exp & 1) ? -bin : bin; /* (-1)^exp * bin */

            result *= factor;
            if (result == 0)         /* zero propagates */
                break;
        }
    }

    if (remaining > 1)                /* remaining large prime (exp = 1) */
    {
        seq_t bin = binom5(1);         /* 5 */
        seq_t factor = -bin;           /* (-1)^1 * 5 = -5 */
        result *= factor;
    }

    return result;
}
