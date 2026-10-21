/*
 * A245194  G.f.: Sum_{k>=0} t^3/((1+t)*(1+t^2)), where t=x^(2^k).
 * https://oeis.org/A245194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{k≥0, 2^k|n} f( (n/2^k) ), where * f(m) = (-1)^{m-3} if
 *            m≥3 and m mod 4 ∈ {0,3}, * 0 otherwise. The function implements
 *            this * directly by testing each power of two that divides n
 * Range:     i8 n <= 94, i16 n <= 94, i32 n <= 94, i64 n <= 94, i128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; (n >> k) > 0; ++k) {
        idx_t m = n >> k;                 /* m = n / 2^k   */
        if ((m << k) != n)                /* skip if not divisible */
            continue;
        if (m < 3)
            continue;                     /* m must be at least 3 */

        idx_t L = m - 3;                  /* L = m - 3       */
        /* f(m) is non‑zero only when L mod 4 is 0 or 1.
         * Those correspond to m mod 4 being 0 or 3.           */
        if ((L & 3ULL) <= 1ULL) {
            seq_t sign = ((L & 1ULL) ? (seq_t)-1 : (seq_t)1);
            sum += sign;
        }
    }
    return sum;
}
