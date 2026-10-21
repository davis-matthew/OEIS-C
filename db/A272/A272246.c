/*
 * A272246  a(n) = Product_{k=0..n} (n^3 + k^3).
 * https://oeis.org/A272246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{k=0..n} (n^3 + k^3)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t ans = 1;
    seq_t n3 = (seq_t)n * (seq_t)n * (seq_t)n;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t k3 = (seq_t)k * (seq_t)k * (seq_t)k;
        ans *= (n3 + k3);
    }
    return ans;
}
