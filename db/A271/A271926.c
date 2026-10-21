/*
 * A271926  Denominator of (Product_{j=0..n-1}
 *          (((2*j+1)*(3*j+4))/((j+1)*(6*j+1))) - 1).
 * https://oeis.org/A271926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of (Product_{j=0..n-1}
 *            (((2*j+1)*(3*j+4))/((j+1)*(6*j+1))) - 1) (offset 1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* safety for out‑of‑spec calls */

    seq_t num = 1, den = 1;

    for (idx_t j = 0; j < n; ++j)
    {
        /* term = tn/td */
        seq_t tn = (seq_t)(2 * j + 1) * (seq_t)(3 * j + 4);
        seq_t td = (seq_t)(j + 1) * (seq_t)(6 * j + 1);

        /* Reduce tn with the current denominator */
        seq_t g = gcd(tn, den);
        if (g != 1) { tn /= g; den /= g; }

        /* Reduce td with the current numerator */
        g = gcd(td, num);
        if (g != 1) { td /= g; num /= g; }

        /* Multiply the reduced factors */
        num *= tn;
        den *= td;

        /* Final reduction for safety */
        g = gcd(num, den);
        if (g != 1) { num /= g; den /= g; }
    }

    /* product - 1 = (num - den) / den   (num > den) */
    seq_t diff = num - den;
    if (diff == 0) return 1;          /* product equals 1 */

    seq_t g = gcd(diff, den);
    return den / g;
}
