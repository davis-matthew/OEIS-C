/*
 * A370753  Antidiagonal products of A319840.
 * https://oeis.org/A370753
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n < 2, otherwise n^2 * 2^(n-2) * (n-1)^(n-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Validated: partial, pass, 19 terms through n = 18 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 1;

    int exp = (int)(n - 2);
    seq_t res = (seq_t)n * (seq_t)n;
    res *= ipow_i32((seq_t)2, exp);
    res *= ipow_i32((seq_t)n - 1, exp);
    return res;
}
