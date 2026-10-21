/*
 * A255932  a(n) is the denominator of Gamma(n+1/2)^2/(2*n*Pi), the value of
 *          an integral with sinh in the denominator.
 * https://oeis.org/A255932
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn
 * Formula:   a(n) = 2^(2*n + 1 + valuation(n, 2))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow2, valuation2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The denominator of Gamma(n+1/2)^2/(2*n*Pi) */

#include "../utils/pow2.h"
#include "../utils/valuation2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset is 1, so n>=1 expected */
    unsigned int v = valuation2(n);
    /* exponent fits in idx_t because n <= large but exponent ≈ 2*n+1+v */
    idx_t exp = (idx_t)(2 * n + 1) + v;
    return pow2(exp);
}
