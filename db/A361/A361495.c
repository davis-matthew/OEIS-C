/*
 * A361495  Number of magic quad squares that can be formed using cards from
 *          Quads-2^n deck, where the first row and column are fixed.
 * https://oeis.org/A361495
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 + 85*(2^n-16) + 43*(2^n-16)*(2^n-32) +
 *            (2^n-16)*(2^n-32)*(2^n-64)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n < 4) return 0;          /* values below the offset are irrelevant */
    seq_t m = pow2(n);            /* compute 2^n */
    seq_t A = m - (seq_t)16;      /* 2^n - 16 */
    seq_t B = m - (seq_t)32;      /* 2^n - 32 */
    seq_t C = m - (seq_t)64;      /* 2^n - 64 */

    seq_t res = (seq_t)10;             /* constant term */
    res += (seq_t)85 * A;              /* 85*(2^n-16) */
    res += (seq_t)43 * A * B;          /* 43*(2^n-16)*(2^n-32) */
    res += A * B * C;                  /* (2^n-16)*(2^n-32)*(2^n-64) */
    return res;
}
