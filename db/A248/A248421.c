/*
 * A248421  a(n) = floor( 1/(n*tan(Pi/n) - Pi) ).
 * https://oeis.org/A248421
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(1/(n*tan(PI/n)-PI))
 * Range:     u8 n <= 51, u16 n <= 823, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

/* Compute a(n) for n ≥ 3.  For n < 3 the sequence is undefined; we
 * return 0. */
seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    /* Convert to double for the transcendental computation.  Double
     * precision is sufficient for all n that produce values fitting in
     * the native integer types used in the test suite. */
    const double PI = acos(-1.0);
    double dn   = (double)n;
    double denom = dn * tan(PI / dn) - PI;

    /* denom is always positive for n ≥ 3.  The 1/denom value is computed
     * in double; add a tiny epsilon before flooring to guard against
     * rounding errors that could otherwise push the value just below the
     * true floor. */
    double inv = 1.0 / denom;
    const double EPS = 1e-12;
    unsigned long long val = (unsigned long long)(floor(inv + EPS));

    /* Convert the unsigned long long result to seq_t.  For the
     * arbitrary‑precision build, seq_t has a constructor from an
     * integer type, so the cast is valid. */
    seq_t result = (seq_t)val;
    return result;
}
