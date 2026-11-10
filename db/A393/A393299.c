/*
 * A393299  a(n) is the number of 2n X 2n matrices over GF(2) such that the
 *          column space and the row space and the null space are all equal.
 * https://oeis.org/A393299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A028362(n)*A002884(n), where A028362(n) =
 *            Product_{i=1..n-1} (2^i + 1) and A002884(n) = Product_{i=1..n}
 *            2^(i-1)*(2^i - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t p = 1; /* 2^(i-1) */
    for (idx_t i = 1; i <= n; ++i) {
        seq_t q = p + p; /* 2^i */
        result = result * p * (q - 1);
        if (i < n) result = result * (q + 1);
        if (result == 0) break;
        p = q;
    }
    return result;
}
