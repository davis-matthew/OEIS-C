/*
 * A262726  Expansion of phi(-x) * psi(x^6) in powers of x where phi(), psi()
 *          are Ramanujan theta functions.
 * https://oeis.org/A262726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{m≥0} c(n−3m(m+1)), where c(e) = 1 if e = 0, 2(−1)^k if
 *            e = k², k > 0, 0 otherwise
 * Range:     i8 n <= 63, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the required floor_sqrt utility. */
#include "../utils/floor_sqrt.h"

/* Helper: coefficient of φ(−x) at exponent e. */
static inline seq_t coeff_phi(seq_t e)
{
    if (e == 0)
        return 1;                           /* k = 0 contributes +1   */

    seq_t k = floor_sqrt(e);                /* integer sqrt of e       */
    if (k * k != e)            /* not a perfect square?  */
        return 0;                   /* no contribution         */

    /* k > 0 case: coefficient is 2(−1)^k.  */
    return ((k & 1) != 0) ? -2 : 2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t ans = 0;
    idx_t m = 0;

    /* Pre‑compute the limit: 3*m*(m+1) ≤ n.  */
    while (1)
    {
        seq_t t = (seq_t)m * (m + 1) * 3;   /* 3m(m+1)                */
        if (t > (seq_t)n)
            break;

        seq_t e = (seq_t)n - t;
        ans += coeff_phi(e);

        ++m;
    }
    return ans;
}
