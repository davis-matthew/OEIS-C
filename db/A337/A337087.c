/*
 * A337087  Number of integer-sided triangles with perimeter n where the
 *          harmonic mean of the side lengths is not an integer.
 * https://oeis.org/A337087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of integer‑sided triangles with perimeter n *
 *            whose harmonic mean of the side lengths is not an integer. *
 *            Computation is done by a direct O(n²) search over all side
 *            triples
 * Range:     u8 n <= 14, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Return 0 for lengths too small to form a triangle */
    if (n < 3)
        return (seq_t)0;

    seq_t count = (seq_t)0;
    idx_t max_a = n / 3;                  /* a <= n/3 ensures a <= b <= c */
    for (idx_t a = 1; a <= max_a; ++a) {
        idx_t max_b = (n - a) / 2;        /* b <= (n-a)/2 ensures b <= c */
        for (idx_t b = a; b <= max_b; ++b) {
            idx_t c = n - a - b;
            if (a + b <= c)               /* triangle inequality */
                continue;
            /* compute harmonic mean: 3*a*b*c / (a*b + a*c + b*c) */
            seq_t ab = (seq_t)a * (seq_t)b;
            seq_t ac = (seq_t)a * (seq_t)c;
            seq_t bc = (seq_t)b * (seq_t)c;
            seq_t denom = ab + ac + bc;
            seq_t numer = (seq_t)3 * ab * (seq_t)c;
            if (denom != 0 && numer % denom != 0)
                ++count;
        }
    }
    return count;
}
