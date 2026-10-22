/*
 * A340336  E.g.f.: Sum_{n>=0} x^n * cosh(2^n*x).
 * https://oeis.org/A340336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = n! * Σ_{m=0}^{⌊n/2⌋} 2^{2m(n-2m)} / (2m)!      (OEIS A340336)
 * The algorithm computes the factorial of n once, then iteratively
 * builds the summands using the ratio
 *
 *     term_m / term_{m-1} = 2^{2n-8m+4} / ((2m-1)(2m))
 *
 * This avoids recomputing factorials for every summand.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* ---------- compute n! ---------- */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;          /* fact = n! */

    /* ---------- summation ---------- */
    seq_t sum  = fact;             /* m = 0 term */
    seq_t term = fact;             /* current term */

    for (idx_t m = 1; m <= n / 2; ++m)
    {
        /* exponent of 2 in the ratio term_{m} / term_{m-1} */
        int64_t exp = (int64_t)2 * (int64_t)n - (int64_t)8 * (int64_t)m + 4;

        /* apply 2^exp: left shift if positive, right shift if negative */
        if (exp >= 0)
            term <<= exp;
        else
            term >>= (int32_t)(-exp);

        /* divide by (2m-1)(2m) */
        seq_t denom = (seq_t)((2 * m - 1) * (2 * m));
        term /= denom;

        /* accumulate */
        sum += term;
    }

    return sum;
}
