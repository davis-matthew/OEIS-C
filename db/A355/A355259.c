/*
 * A355259  Triangle read by rows. Row k are the coefficients of the
 *          polynomials (sorted by ascending powers) which interpolate the
 *          points (n, A355257(n, k+1)) for n = 0..k.
 * https://oeis.org/A355259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^i in row k (j=k+1) = Sum_{m=1..j}
 *            binomial(j,m)*(m-1)!*(2^m-1)*c(j-m,i), c = unsigned Stirling
 *            numbers of the first kind; n = k(k+1)/2 + i
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 44 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n) {
    idx_t k = 0, base = 0;
    while (n >= base + k + 1) { base += k + 1; k++; }
    idx_t i = n - base;
    idx_t j = k + 1;
    idx_t w = k + 1;
    seq_t *c = mem_new(seq_t, w * w);
    c[0] = 1;
    for (idx_t t = 1; t < w; t++) {
        for (idx_t s = 0; s <= t; s++) {
            seq_t v = 0;
            if (s > 0) v += c[(t - 1) * w + (s - 1)];
            if (s < t) v += (seq_t)(t - 1) * c[(t - 1) * w + s];
            c[t * w + s] = v;
        }
    }
    seq_t sum = 0;
    seq_t fall = 1;
    seq_t pw = 1;
    for (idx_t m = 1; m <= j; m++) {
        fall *= (seq_t)(j - m + 1);
        pw *= 2;
        seq_t coef = fall / (seq_t)m;
        idx_t t = j - m;
        seq_t st = (i <= t) ? c[t * w + i] : (seq_t)0;
        sum += coef * (pw - 1) * st;
    }
    mem_free(c);
    return sum;
}
