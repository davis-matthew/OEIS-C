/*
 * A293914  Number of linear chord diagrams having n chords and minimal chord
 *          length one.
 * https://oeis.org/A293914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A293914: a(n) = number of linear chord diagrams with n chords
   having at least one chord of length one.
   Formula (inclusion–exclusion):
        a(n) = Σ_{t=1..n} (-1)^{t+1} * C(2n - t, t) * (2n - 2t - 1)!!   */
#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* offset is 1, caller may not ask for n=0 */

    /* df0 = (2n-1)!!  (double factorial of odd numbers up to 2n-1) */
    seq_t df_current = 1;
    for (idx_t i = 1; i <= n; ++i)
        df_current *= (seq_t)(2 * i - 1);

    seq_t sumOdd = 0;
    seq_t sumEven = 0;

    for (idx_t t = 1; t <= n; ++t) {
        /* divide by the next odd factor to shift df from (2(n-t+1)-1)!!
           to (2(n-t)-1)!!                                         */
        idx_t odd = 2 * n - 2 * t + 1;  /* odd ≥ 1 for all t ≤ n      */
        df_current /= (seq_t)odd;

        seq_t bin = binom(2 * n - t, t);
        seq_t term = bin * df_current;
        if ((t & 1U) != 0)
            sumOdd += term;
        else
            sumEven += term;
    }

    return sumOdd - sumEven;          /* the final count is non‑negative      */
}
