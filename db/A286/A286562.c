/*
 * A286562  Transpose of square array A286561.
 * https://oeis.org/A286562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A286561(n) transposed: reading A(r,c) by antidiagonals
 *            with r = m-pos+1, c = pos; A(r,1) = 1, A(r,c) = max e with c^e |
 *            r for c > 1
 * Range:     u8 n <= 7875, u16 n <= 7875, u32 n <= 7875, u64 n <= 7875,
 *            u128 n <= 7875, big any n
 * Validated: standard, pass, 136 terms through n = 7845 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = 1;
    idx_t tri = 1;
    while (tri < n) {
        ++m;
        tri += m;
    }
    idx_t pos = n - (tri - m);
    idx_t row = m - pos + 1;
    idx_t col = pos;

    if (col == 1)
        return (seq_t)1;

    unsigned e = 0;
    while (row % col == 0) {
        row /= col;
        ++e;
    }
    return (seq_t)e;
}
