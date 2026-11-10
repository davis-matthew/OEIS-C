/*
 * A278080  Expansion of e.g.f. (1/4!)*sin^4(x)/cos(x) (coefficients of even
 *          powers only).
 * https://oeis.org/A278080
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = [x^(2n)/(2n)!] (1/4!)*sin^4(x)/cos(x). * Multiply by
 *            cos(x): Sum_{k=0..n} binomial(2n,2k)*(-1)^(n-k)*a(k) = s(n),
 *            where * s(n) = [x^(2n)/(2n)!] sin^4(x)/24 = (-1)^n * 4^(n-2) *
 *            (4^(n-1)-1)/3 for n >= 2, * and s(0) = s(1) = 0. So * a(n) =
 *            s(n) - Sum_{k=0..n-1} binomial(2n,2k)*(-1)^(n-k)*a(k...
 * Range:     i8 n <= 4, i16 n <= 5, i32 n <= 8, i64 n <= 12, i128 n <= 16,
 *            big any n
 * Validated: partial, pass, 17 terms through n = 16 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

static seq_t *A278080_vals = NULL;
static idx_t A278080_have = 0;

static seq_t A278080_s(idx_t m) {
    if (m < 2) return 0;
    seq_t t = 1;                 /* 4^(m-2) */
    for (idx_t i = 0; i + 2 < m; i++) t *= 4;
    seq_t g = 0, p = 1;          /* (4^(m-1)-1)/3 = Sum_{j=0..m-2} 4^j */
    for (idx_t j = 0; j + 1 < m; j++) { g += p; p *= 4; }
    seq_t r = t * g;
    if (m & 1) r = -r;
    return r;
}

seq_t a(idx_t n) {
    if (n < A278080_have)
        return A278080_vals[n];

    idx_t N = n;
    if (N < 16) N = 16;
    idx_t R = 2 * N;

    seq_t *vals = mem_new(seq_t, (size_t)(N + 1));
    for (idx_t i = 0; i < A278080_have; i++) vals[i] = A278080_vals[i];

    seq_t *row = mem_new(seq_t, (size_t)(R + 2));
    row[0] = 1;
    /* row currently is Pascal row 0 */
    for (idx_t r = 0; r <= R; r++) {
        if (r > 0) {
            for (idx_t j = r; j >= 1; j--)
                row[j] += row[j - 1];
        }
        if ((r & 1) == 0) {
            idx_t m = r / 2;
            if (m >= A278080_have && m <= N) {
                seq_t v = A278080_s(m);
                for (idx_t k = 0; k < m; k++) {
                    seq_t term = row[2 * k] * vals[k];
                    if ((m - k) & 1) v += term;
                    else v -= term;
                }
                vals[m] = v;
            }
        }
    }
    mem_free(row);

    if (A278080_vals != NULL) mem_free(A278080_vals);
    A278080_vals = vals;
    A278080_have = N + 1;
    return A278080_vals[n];
}
