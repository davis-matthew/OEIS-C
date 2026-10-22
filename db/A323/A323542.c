/*
 * A323542  a(n) = Product_{k=0..n} (k^4 + (n-k)^4).
 * https://oeis.org/A323542
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (k^4 + (n-k)^4)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t kk = (seq_t)k;
        seq_t nk = (seq_t)(n - k);
        seq_t term = kk * kk * kk * kk + nk * nk * nk * nk;
        result *= term;
    }
    return result;
}
