/*
 * A344421  Number of lattice points (x,y) in the region bounded by 0 < y <
 *          n*sin(x*Pi/n) for 0 < x < n.
 * https://oeis.org/A344421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{x=1}^{n-1} ⌊ n*sin(x*π/n) - ε ⌋, where ε = 1e‑12
 *            guarantees the correct integer count for all n ≥ 1
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n <= 1U)
        return 0;

    seq_t total = 0;
    const double PI   = acos(-1.0);     /* π */
    const double dn   = (double)n;
    const double eps  = 1e-12;          /* safety margin for rounding */

    for (idx_t x = 1; x < n; ++x) {
        double s = dn * sin((double)x * PI / dn);
        /* s > 0, subtract eps to handle the case where s is an integer.
           Casting to unsigned int truncates toward zero, i.e. floor. */
        unsigned int count = (unsigned int)(s - eps);
        total += count;
    }

    return total;
}
