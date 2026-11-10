/*
 * A265159  Rectangular array A read by upward antidiagonals in which the
 *          entry in row n and column k is defined by A(n,k) = 5 +
 *          9*A005836(2^(k - 1)*(2 n - 1)), n,k >= 1.
 * https://oeis.org/A265159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(m) = A(n,k) read by upward antidiagonals, where A(n,k) = 5 +
 *            9*A005836(2^(k-1)*(2n-1)). Since 2^(k-1)*(2n-1) - 1 in binary is
 *            (n-1) followed by a 0 and then k-1 ones, this gives
 *            A005836(2^(k-1)*(2n-1)) = B3(n-1)*3^k + (3^(k-1)-1)/2, where
 *            B3(x) reads the binary digits of x as a base-3 number
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t m) {
    idx_t d = 1;
    while (d * (d + 1) / 2 < m) d++;      /* m lies in (d(d-1)/2, d(d+1)/2] */
    idx_t i = m - d * (d - 1) / 2;        /* position 1..d within the diagonal */
    idx_t row = d - i + 1;                /* upward: start at the bottom row */
    idx_t col = i;

    /* B3(row-1) */
    idx_t x = row - 1;
    seq_t v = 0, p = 1;
    while (x) {
        if (x & 1) v += p;
        p *= 3;
        x >>= 1;
    }
    /* shift left by col places in base 3 */
    for (idx_t t = 0; t < col; t++) v *= 3;
    /* add the col-1 trailing ones: 3^0 + ... + 3^(col-2) */
    seq_t q = 1;
    for (idx_t t = 1; t < col; t++) {
        v += q;
        q *= 3;
    }
    return v * 9 + 5;
}
