/*
 * A249347  The exponent of the highest power of 7 dividing the product of the
 *          elements on the n-th row of Pascal's triangle.
 * https://oeis.org/A249347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} (2*i - n - 1) * v_7(i), where v_7(i) is the
 *            exponent of the highest power of 7 dividing i
 * Range:     u8 n <= 146, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   v7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/v7.h"

seq_t a(idx_t n)
{
    /* Sum may temporarily go negative, use signed 64‑bit for safety. */
    int64_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        int v = v7(k);
        int64_t diff = (int64_t)2 * (int64_t)k - (int64_t)n - 1;
        sum += diff * (int64_t)v;
    }
    return (seq_t)sum;
}
