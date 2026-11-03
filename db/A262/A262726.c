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
 * Validated: standard, pass, 97 terms through n = 9794 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the required floor_sqrt utility. */
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

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
