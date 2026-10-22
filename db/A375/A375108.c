/*
 * A375108  Expansion of Sum_{k in Z} x^(3*k) / (1 - x^(7*k+3)).
 * https://oeis.org/A375108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k in Z} x^{3k}/(1 - x^{7k+3}) expanded as a power
 *            series, that is, the coefficient of x^n in ∏_{k>0} (1-x^{7k})^2
 *            (1-x^{7k-1})(1-x^{7k-6})/ ((1-x^{7k-3})(1-x^{7k-4}))^2
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute a(n) for n >= 0.  The function works for any integer
   implementation of seq_t and idx_t.  It runs in linear time in n. */
seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Bounds for k:  ceil(-(n+3)/4) <= k <= floor(n/3)  */
    /* Using signed 64‑bit arithmetic is safe for the n sizes
       that appear in the tests (up to some 10⁴ or 10⁵).     */
    int64_t k_min = -((int64_t)((n + 3ULL) / 4ULL));
    int64_t k_max = (int64_t)(n / 3ULL);

    for (int64_t k = k_min; k <= k_max; ++k)
    {
        /* Use 128‑bit to avoid overflow for intermediate calculations
           when n is large.  All arithmetic here is exact. */
        __int128 d = (__int128)7 * k + 3;          /* 7k+3  */
        __int128 t = (__int128)n - (__int128)3 * k;/* n-3k   */

        if (d > 0)
        {
            /* Positive denominator: contribution +1 if n-3k is a
               non‑negative multiple of d.                           */
            if (t >= 0 && t % d == 0)
                result += 1;
        }
        else /* d < 0 */
        {
            /* Negative denominator contributes -1 for each t>0
               with (n-3k) a multiple of |d|.                        */
            __int128 pos_d = -d;
            if (t > 0 && t % pos_d == 0)
                result -= 1;
        }
    }

    return result;
}
