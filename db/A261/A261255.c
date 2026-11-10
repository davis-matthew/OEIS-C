/*
 * A261255  Where MU-numbers (cf. A007335) occur in A003586 (3-smooth
 *          numbers).
 * https://oeis.org/A261255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = index k such that A003586(k) is the n-th MU-number
 *            A007335(n), i.e. the n-th 3-smooth number 2^v*3^w (in increasing
 *            order) with v=1 or w=1 or both v,w odd; computed by generating
 *            3-smooth numbers as exponent pairs (Hamming merge)
 * Range:     u8 n <= 80, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 850 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static int A261255_ok(idx_t v, idx_t w) {
    return v == 1 || w == 1 || ((v & 1) && (w & 1));
}

static idx_t A261255_try(idx_t N, idx_t n) {
    idx_t *e2 = (idx_t *)malloc((size_t)N * sizeof(idx_t));
    idx_t *e3 = (idx_t *)malloc((size_t)N * sizeof(idx_t));
    idx_t i2 = 0, i3 = 0, k, cnt = 0, res = 0;
    const double L2 = 0.69314718055994530942, L3 = 1.09861228866810969140;
    e2[0] = 0; e3[0] = 0;
    for (k = 1; k < N; k++) {
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
        if (A261255_ok(e2[k], e3[k])) {
            cnt++;
            if (cnt == n) { res = k + 1; break; }
        }
    }
    free(e2);
    free(e3);
    return res;
}

seq_t a(idx_t n) {
    idx_t N = 4 * n + 100;
    for (;;) {
        idx_t r = A261255_try(N, n);
        if (r) return (seq_t)r;
        N *= 2;
    }
}
