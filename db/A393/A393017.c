/*
 * A393017  Triangle read by rows: T(n, k) = denominator(8*n^3*k/(k^2+4*n^2)).
 * https://oeis.org/A393017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, frac
 * Formula:   a(n) = denom( 8*n^3*k / (k^2 + 4*n^2) ), where (n,k) are
 *            obtained from the n‑th entry of the triangle read by rows (row n
 *            contains k = 1..n)
 * Range:     u8 n <= 5, u16 n <= 54, u32 n <= 3240, u64 n <= 3240,
 *            u128 n <= 3240, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/gcd.h"

seq_t a(idx_t idx) {
    /* idx is 1‑based; for idx < 1 return 0 (unused). */
    if (idx == 0) return 0;

    /* Find row number n such that n(n+1)/2 >= idx. */
    idx_t disc = (idx_t)floorl(sqrtl((long double)(1 + 8ULL * idx)));
    idx_t n = (disc - 1) / 2;
    if (n * (n + 1) / 2 < idx) ++n;

    /* Column k in that row. */
    idx_t prev = n * (n - 1) / 2;
    idx_t k = idx - prev;   /* 1‑based column */

    /* Compute numerator and denominator of the fraction. */
    seq_t N = (seq_t)n;
    seq_t K = (seq_t)k;
    seq_t denom = K * K + (seq_t)4 * N * N;
    seq_t numer = (seq_t)8 * N * N * N * K;

    /* Reduced denominator. */
    seq_t g = gcd(numer, denom);
    return denom / g;
}
