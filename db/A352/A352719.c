/*
 * A352719  Indices k of tribonacci numbers T(k) such that T(k+1) -
 *          (tribonacci constant)*T(k) is nonnegative.
 * https://oeis.org/A352719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 0 such that T(k+1) - tribConst·T(k) ≥ 0;
 *            a(1)=0, a(2)=1. For k≥2, the condition is equivalent to (a +
 *            b·k) mod (2π) ∈ (π/2, 3π/2) where a = 2.5361552893…
 *            (arg(c2*(ψ‑α))) and b = 4.106951763… (arg(α)). The function
 *            scans k=2,3,… and counts the qualifying k+1 until the nth
 *            term...
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;

    /* Constants (high‑precision) */
    static const long double pi      = acosl(-1.0L);
    static const long double two_pi  = 2.0L * pi;
    static const long double a_const = 2.5361552893L;   /* arg(c2*(ψ‑α)) */
    static const long double b_const = 4.106951763L;   /* arg(α)          */

    /* Boundaries for the angular interval */
    static const long double half_pi      = pi / 2.0L;
    static const long double three_half_pi = 3.0L * pi / 2.0L;

    /* We scan k starting from 2.  Count how many indices k+1 are
       acceptable until we reach the (n‑2)-th such occurrence. */
    idx_t found = 0;
    idx_t k = 2;
    long double theta = a_const + k * b_const;   /* initial value for k=2 */

    while (1)
    {
        /* Normalize theta to [0, 2π) */
        if (theta >= two_pi)
            theta -= two_pi;
        else if (theta < 0.0L)
            theta += two_pi;

        /* Check if theta is in the required half‑circle */
        if (theta > half_pi && theta < three_half_pi)
        {
            ++found;
            if (found == n - 2)       /* corresponding index is k+1 */
                return (seq_t)(k + 1);
        }

        /* Advance to the next k */
        ++k;
        theta += b_const;
        /* Keep theta bounded after addition */
        if (theta >= two_pi)
            theta -= two_pi;
    }
}
