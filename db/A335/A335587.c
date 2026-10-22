/*
 * A335587  a(n) is the sum of the numbers k such that 0 <= k <= n and n AND k
 *          = 0 (where AND denotes the bitwise AND operator).
 * https://oeis.org/A335587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{k=0..n, (n & k) == 0} k
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    for (idx_t k = 0; k <= n; ++k) {
        if ((n & k) == 0) {
            sum += (seq_t)k;
        }
    }
    return sum;
}
