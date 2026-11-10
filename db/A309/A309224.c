/*
 * A309224  Bisection A164733(2*n+1).
 * https://oeis.org/A309224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   a(n) = A164733(2*n-1), where A164733 has g.f. x*N(x)/D(x), D(x)
 *            = 1 + sum_{k=1}^{31} d_k x^k, computed via the linear recurrence
 *            A(m) = B[m] - sum_k d_k*A(m-k), A(0)=0, A(1)=1, A(2)=0, A(3)=1
 * Range:     u8 n <= 35, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static const int a309224_d[32] = {
    0, 0, -1, 0, 0, 0, -1, 0, 1, -1, 0, 1, 0, 0, -1, 1,
    1, -1, 0, 0, 1, 0, -1, 1, 0, -1, 0, 0, 0, -1, 0, 1
};

static const int a309224_B[34] = {
    0, 1, 0, 0, 1, -1, 1, -1, 0, 2, -1, 0, 1, -1, 2, -3,
    2, 1, -1, 2, 0, 1, 1, -1, 2, -1, 0, 1, -1, 1, -1, 0,
    1, -1
};

seq_t a(idx_t n)
{
    size_t m = (size_t)(2 * n - 1);
    seq_t *A = mem_new(seq_t, m + 1);
    seq_t result;
    size_t i;
    int k;
    for (i = 1; i <= m; ++i) {
        seq_t v = 0;
        if (i <= 33) {
            int b = a309224_B[i];
            if (b > 0) v += (seq_t)b;
            else if (b < 0) v -= (seq_t)(-b);
        }
        for (k = 1; k <= 31 && (size_t)k <= i; ++k) {
            int c = a309224_d[k];
            if (c == 1) v -= A[i - k];
            else if (c == -1) v += A[i - k];
        }
        A[i] = v;
    }
    result = A[m];
    mem_free(A);
    return result;
}
