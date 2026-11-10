/*
 * A347839  An array of the positive integers congruent to 2 modulo 3
 *          (A016789), read by antidiagonals upwards, giving the present
 *          triangle.
 * https://oeis.org/A347839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   t(k,n) = a(k-n,n), 0<=n<=k-1, with a(m,n) = 4^n*(3*m - 1 +
 *            3*floor(m/3)), read by antidiagonals
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t idx)
{
    if (idx == 0)
        return 0;

    idx_t k = 1;
    idx_t before = 0;                 /* terms in rows before row k */
    while (before + k < idx) {
        before += k;
        ++k;
    }
    idx_t n = idx - before - 1;       /* 0..k-1 */
    idx_t m = k - n;                  /* array row, >= 1 */

    seq_t val = (seq_t)m * 3 + (seq_t)(m / 3) * 3 - 1;
    for (idx_t i = 0; i < n; ++i)
        val *= 4;
    return val;
}
