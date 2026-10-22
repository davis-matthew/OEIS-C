/*
 * A376324  (1/6) times obverse convolution (4)**(2^n + 1); see Comments.
 * https://oeis.org/A376324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Π_{k=1..n} (2^k + 5), a(0) = 1 Recurrence: a(n) = a(n‑1)
 *            * (2^n + 5) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t k = (idx_t)1; k <= n; ++k)
    {
        seq_t term = pow2(k) + (seq_t)5;
        result *= term;
    }
    return result;
}
