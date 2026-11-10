/*
 * A255557  Square array A(row,col): A(1,1) = 1, A(1,col) = A055938(col-1),
 *          and for row > 1: A(row,col) = A005187(1+A(row-1,col)).
 * https://oeis.org/A255557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A(1,1)=1, A(1,col)=A055938(col-1),
 *            A(row,col)=A005187(1+A(row-1,col)); read by antidiagonals
 * Range:     u8 n <= 42, u16 n <= 150, u32 n <= 558, u64 n <= 2142,
 *            u128 n <= 8382, big any n
 * Validated: standard, pass, 94 terms through n = 9264 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A005187_f(seq_t n)
{
    seq_t r = 0;
    while (n > 0) {
        r += n;
        n >>= 1;
    }
    return r;
}

/* m-th positive integer not in A005187 (A055938) */
static seq_t A055938_m(idx_t m)
{
    seq_t c = 1, k = 0;
    idx_t cnt = 0;
    for (;; ++c) {
        while (A005187_f(k) < c) ++k;
        if (A005187_f(k) != c) {
            ++cnt;
            if (cnt == m) return c;
        }
    }
}

seq_t a(idx_t n)
{
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) ++d;
    idx_t k = n - d * (d - 1) / 2;
    idx_t row = k;
    idx_t col = d + 1 - k;
    seq_t x;
    if (col == 1) x = 1;
    else x = A055938_m(col - 1);
    for (idx_t i = 1; i < row; ++i) {
        x = A005187_f(x + 1);
    }
    return x;
}
