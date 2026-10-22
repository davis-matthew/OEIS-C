/*
 * A375149  Expansion of Sum_{k in Z} x^k / (1 - x^(7*k+4)).
 * https://oeis.org/A375149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A375149
   cech on term expansion:
   a(n) =  # {k ≥ 0 | n = 4k + m(7k+1) for some integer m ≥ 0}
           - # {t ≥ 1 | 3t-1 ≤ n and n = (3t-1) + m(7t-1) for some integer m ≥ 0}
   This formula follows directly from the series
   \sum_{k∈ℤ} \frac{x^{4k}}{1 - x^{7k+1}}.  */

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* contributions from k ≥ 0 (positive terms) */
    for (idx_t k = 0; k <= n / 4; ++k) {
        idx_t d = 7 * k + 1;               /* divisor 7k+1  */
        idx_t rem = (n - 4 * k) % d;       /* remainder of (n-4k) divided by d */
        if (rem == 0)                      /* m = (n-4k)/d is integer ≥ 0 */
            res += (seq_t)1;
    }

    /* contributions from k < 0 (negative terms) represented by t = -k */
    for (idx_t t = 1; 3 * t - 1 <= n; ++t) {
        idx_t offset = 3 * t - 1;          /* base exponent for this t   */
        idx_t step   = 7 * t - 1;          /* step between exponents     */
        idx_t rem = (n - offset) % step;
        if (rem == 0)                      /* term exists, coefficient is -1 */
            res -= (seq_t)1;
    }

    return res;
}
