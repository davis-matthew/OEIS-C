/*
 * A292436  Array T read by antidiagonals: T(m,n) is the number of lattice
 *          walks of minimal length from (0,0) to (m,n) using steps in
 *          directions from {(1,0), (0,1), (2,1), (1,2)}.
 * https://oeis.org/A292436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, tabl
 * Formula:   a(n) = T(m,n) read by antidiagonals of the minimal‑length
 *            lattice walk array with steps {(1,0),(0,1),(2,1),(1,2)}.
 *            Coordinates (m,n) are obtained from index n by d =
 *            floor((sqrt(8*n+1)-1)/2) and offset = n - d*(d+1)/2; m = offset,
 *            n' = d - offset. T(m,n') is evaluated by the piecewise binomial
 *            fo...
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* determine diagonal d */
    double tmp = sqrt(8.0 * (double)n + 1.0);
    idx_t d   = (idx_t)((tmp - 1.0) / 2.0);

    /* offset within the diagonal */
    idx_t prev   = d * (d + 1) / 2;
    idx_t offset = n - prev;
    idx_t m      = offset;          /* horizontal coordinate */
    idx_t vp     = d - offset;      /* vertical coordinate */

    if (m >= 2 * vp) {
        return binomial(m - vp, vp);
    } else if (m <= vp / 2) {
        return binomial(vp - m, m);
    } else {
        idx_t sum = m + vp;
        idx_t q   = sum / 3;
        idx_t r   = sum % 3;
        seq_t a1  = binomial(q + r, r);
        seq_t a2  = binomial(q + r, m - q);
        return a1 * a2;
    }
}
