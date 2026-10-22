/*
 * A399943  Number of endofunctions on [n] having at least two isolated fixed
 *          points.
 * https://oeis.org/A399943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{j=2..n} (-1)^j * (j-1) * binomial(n,j) * (n-j)^(n-j)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 0.  For n = 0 or 1 it is 0. */
    if (n < 2)
        return 0;

    seq_t pos = 0;   /* sum of terms with even j  (positive sign) */
    seq_t neg = 0;   /* sum of terms with odd j   (negative sign) */

    idx_t j;
    for (j = 2; j <= n; ++j)
    {
        /* binomial(n,j) is provided by the utils library. */
        seq_t term = binomial(n, j);           /* C(n, j)          */
        term = term * (seq_t)(j - 1);          /* *(j-1)           */
        seq_t powv = pow_uint((uint64_t)(n - j), (idx_t)(n - j));
                                            /* (n-j)^(n-j)      */
        term = term * powv;                    /* full term        */

        /* (-1)^j: even j → positive, odd j → negative. */
        if ((j & 1) == 0)
            pos = pos + term;    /* even j */
        else
            neg = neg + term;    /* odd j  */
    }

    /* The final result is always non‑negative, so pos ≥ neg. */
    return pos - neg;
}
