/*
 * A309223  Bisection A164733(2*n).
 * https://oeis.org/A309223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = A164733(2n), where A164733 has g.f. x*P(x)/Q(x) (linear
 *            recurrence of order 31): A164733(m) = B[m] - Sum_{k=1..31}
 *            d_k*A164733(m-k), d_k = coefficients of the denominator, B =
 *            numerator coefficients; A164733(1)=1, A164733(2)=0,
 *            A164733(3)=1, A164733(4)=1
 * Range:     u8 n <= 31, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static const int A309223_d[32] = {
    0, 0, -1, 0, 0, 0, -1, 0, 1, -1, 0, 1, 0, 0, -1, 1,
    1, -1, 0, 0, 1, 0, -1, 1, 0, -1, 0, 0, 0, -1, 0, 1
};

static const int A309223_B[34] = {
    0, 1, 0, 0, 1, -1, 1, -1, 0, 2, -1, 0, 1, -1, 2, -3,
    2, 1, -1, 2, 0, 1, 1, -1, 2, -1, 0, 1, -1, 1, -1, 0,
    1, -1
};

seq_t a(idx_t n)
{
    size_t m = (size_t)n * 2;
    seq_t *c = mem_new(seq_t, m + 1);
    seq_t result;
    size_t i;
    int k;
    for (i = 1; i <= m; ++i) {
        seq_t acc = 0;
        for (k = 1; k <= 31 && (size_t)k < i; ++k) {
            int dk = A309223_d[k];
            if (dk == -1) acc += c[i - k];
            else if (dk == 1) acc -= c[i - k];
        }
        if (i <= 33) {
            int b = A309223_B[i];
            if (b > 0) acc += (seq_t)b;
            else if (b < 0) acc -= (seq_t)(-b);
        }
        c[i] = acc;
    }
    result = c[m];
    mem_free(c);
    return result;
}
