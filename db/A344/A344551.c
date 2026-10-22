/*
 * A344551  a(n) = Sum_{k=1..n} k^floor((n-k)/k).
 * https://oeis.org/A344551
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k^floor((n-k)/k)
 * Range:     u8 n <= 15, u16 n <= 31, u32 n <= 62, u64 n <= 125,
 *            u128 n <= 245, big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t t = n / k;              /* t = floor(n/k) */
        seq_t val = pow_int((seq_t)k, t - 1);
        sum += val;
    }
    return sum;
}
