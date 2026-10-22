/*
 * A321556  a(n) = Sum_{d|n} (-1)^(n/d+1)*d^11.
 * https://oeis.org/A321556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∑_{d|n} (-1)^{n/d+1} * d^11
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 56, u128 n <= 3183,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* helper: compute d^11 using only multiplication */
static seq_t pow11(seq_t d)
{
    seq_t r = 1;
    for (int i = 0; i < 11; ++i) r *= d;
    return r;
}

/* the sequence function */
seq_t a(idx_t n)
{
    /* offset is 1; we assume n >= 1 */
    seq_t pos = 0; /* positive contributions */
    seq_t neg = 0; /* negative contributions */

    /* enumerate divisors up to sqrt(n) */
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d != 0) continue;

        /* divisor pair: d and n/d */
        idx_t d1 = d;
        idx_t d2 = n / d;

        /* process d1 */
        {
            idx_t k = n / d1;                /* n/d1 */
            bool positive = ((k + 1) & 1) == 0;   /* (-1)^(k+1) positive? */
            seq_t val = pow11((seq_t)d1);
            if (positive) pos += val;
            else          neg += val;
        }

        /* process d2 if distinct */
        if (d2 != d1)
        {
            idx_t k = n / d2;                /* n/d2 */
            bool positive = ((k + 1) & 1) == 0;
            seq_t val = pow11((seq_t)d2);
            if (positive) pos += val;
            else          neg += val;
        }
    }

    /* result is positive (proved by the sequence definition) */
    return pos - neg;
}
