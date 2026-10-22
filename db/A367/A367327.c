/*
 * A367327  a(n) = Sum_{(n - k) does not divide n, 0 <= k < n} k^2.
 * https://oeis.org/A367327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0}^{n-1, (n-k) does not divide n} k^2
 * Range:     u8 n <= 10, u16 n <= 60, u32 n <= 2346, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d < n; ++d) {
        if (n % d != 0) {
            idx_t diff = n - d;
            seq_t dsq = (seq_t)diff * (seq_t)diff;
            sum += dsq;
        }
    }
    return sum;
}
