/*
 * A383579  a(n) is the largest number t such that there exist numbers i,j,k
 *          such that, for all m <= t, there exist integers x,y,z with x*i +
 *          y*j + z*k = m and |x|+|y|+|z| <= n.
 * https://oeis.org/A383579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{m=0..n-1} (4*n^2*m - 12*n*m^2 + 8*m^3 + 3*n^2 -
 *            11*n*m + 10*m^2 - n + 3*m) (conjectured formula valid for n > 5;
 *            lower values provided explicitly)
 * Range:     u8 n <= 7, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Small values known from the sequence b-file. */
    static const seq_t small_vals[6] = {0, 3, 10, 24, 46, 80};

    if (n <= 5) {
        return small_vals[n];
    }

    /* Use signed long long for intermediate arithmetic – values comfortably fit for the
       required range (n up to at least 10 000). */
    idx_t i;
    seq_t max_val = 0;

    const long long nn = (long long)n;
    const long long c_term_const = 3 * nn * nn - nn;            /* 3*n^2 - n */
    const long long coeff_m = 4 * nn * nn - 11 * nn + 3;        /* 4*n^2 - 11*n + 3 */

    for (i = 0; i < n; ++i) {
        const long long mm = (long long)i;
        const long long mm2 = mm * mm;
        const long long mm3 = mm2 * mm;

        /* Compute polynomial value */
        long long val = 8 * mm3
                       - 12 * nn * mm2
                       + 10 * mm2
                       + coeff_m * mm
                       + c_term_const;

        if (i == 0 || val > (long long)max_val) {
            max_val = (seq_t)val;
        }
    }

    return max_val;
}
