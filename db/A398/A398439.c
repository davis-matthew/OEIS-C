/*
 * A398439  Antidiagonal sums of the array A398438.
 * https://oeis.org/A398439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} A(n-k,k), where A(0,k) = 0, A(m,k) =
 *            A(m-1,k)^2 + k (A398438)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 8, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A398438_a(idx_t i, idx_t j)
{
    seq_t val = 0;
    for (idx_t r = 1; r <= i; ++r)
        val = val * val + (seq_t)j;
    return val;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k)
        sum += A398438_a(n - k, k);
    return sum;
}
