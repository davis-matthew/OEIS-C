/*
 * A307675  L.g.f.: log(Product_{k>=1} 1/(1 - x^k/(1 + x))) = Sum_{k>=1}
 *          a(k)*x^k/k.
 * https://oeis.org/A307675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 8, i16 n <= 15, i32 n <= 29, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A307675
 * Formula: a(n) = Σ_{d=1}^{n} Σ_{m|d} (n / m) * (-1)^{n-d} * binom(m + n - d - 1, n - d)
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int sgn = ((n - d) & 1) ? -1 : 1;   /* (-1)^{n-d} */
        /* enumerate all divisors m of d */
        for (idx_t m = 1; m * m <= d; ++m) {
            if (d % m != 0) continue;

            /* first divisor m */
            seq_t bin = binom(m + n - d - 1, n - d);
            seq_t term = bin * ((seq_t)n) / (seq_t)m;
            if (sgn < 0) sum -= term; else sum += term;

            /* if m and d/m are distinct, handle the second divisor */
            idx_t m2 = d / m;
            if (m2 != m) {
                seq_t bin2 = binom(m2 + n - d - 1, n - d);
                seq_t term2 = bin2 * ((seq_t)n) / (seq_t)m2;
                if (sgn < 0) sum -= term2; else sum += term2;
            }
        }
    }
    return sum;
}
