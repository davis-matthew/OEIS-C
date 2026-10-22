/*
 * A308313  a(n) = Sum_{k=1..n} (-1)^(n-k) * k^n * floor(n/k).
 * https://oeis.org/A308313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} (-1)^(n-k) * k^n * floor(n/k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A308313 using an O(n log n) algorithm based on the
 * alternative expression
 *   a(n) = Σ_{d=1..n} (-1)^(n-d) * d^n * floor(n/d).
 * For each d we evaluate d^n by repeated squaring with big_pow,
 * scale it by floor(n/d) with mul_small, and accumulate a positive
 * and a negative partial sum.  The final result is the difference
 * of these two unsigned sums, which is safe because a(n) is always
 * non‑negative.  All arithmetic is performed with the seq_t type
 * supplied by the build; utils provide big_pow and mul_small. */

#include "../utils/big_pow.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t pos = 0;              /* sum of terms with (+) sign   */
    seq_t neg = 0;              /* sum of terms with (-) sign   */

    for (idx_t d = 1; d <= n; ++d)
    {
        /* d^n (using exponentiation by squaring)      */
        seq_t d_pow = big_pow((seq_t)d, (seq_t)n);

        /* multiply by floor(n/d) (small integer)      */
        unsigned int factor = (unsigned)(n / d);
        seq_t term = mul_small(d_pow, factor);

        /* add to the appropriate partial sum           */
        if ( ((n - d) & 1) == 0 )        /* even exponent difference   */
            pos += term;
        else
            neg += term;
    }

    /* a(n) = pos - neg; pos >= neg for all n ≥ 1       */
    return pos - neg;
}
