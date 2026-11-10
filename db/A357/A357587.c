/*
 * A357587  If k > 1 and k divides DedekindPsi(k) then A358015(k)/2 is a term
 *          of this sequence.
 * https://oeis.org/A357587
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let 2^x*3^y be the n-th 3-smooth number (A003586). Then a(n) =
 *            3^y if x = 0, else 2^(x+1)*3^y. This is A358015(6*2^x*3^y)/2
 * Range:     u8 n <= 21, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t *e2 = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    idx_t *e3 = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    idx_t i2 = 0, i3 = 0, k, t;
    const double L2 = 0.69314718055994530942, L3 = 1.09861228866810969140;
    seq_t r = 1;
    e2[0] = 0; e3[0] = 0;
    for (k = 1; k < n; k++) {
        idx_t a2 = e2[i2] + 1, b2 = e3[i2];
        idx_t a3 = e2[i3], b3 = e3[i3] + 1;
        if (a2 == a3 && b2 == b3) {
            e2[k] = a2; e3[k] = b2; i2++; i3++;
        } else {
            double v2 = (double)a2 * L2 + (double)b2 * L3;
            double v3 = (double)a3 * L2 + (double)b3 * L3;
            if (v2 < v3) { e2[k] = a2; e3[k] = b2; i2++; }
            else { e2[k] = a3; e3[k] = b3; i3++; }
        }
    }
    idx_t x = e2[n - 1], y = e3[n - 1];
    if (x > 0) {
        for (t = 0; t < x + 1; t++) r *= 2;
    }
    for (t = 0; t < y; t++) r *= 3;
    free(e2);
    free(e3);
    return r;
}
