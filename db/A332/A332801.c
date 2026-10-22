/*
 * A332801  a(n) is the number of even results of n mod k, for 1 < k < n.
 * https://oeis.org/A332801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of k (2 ≤ k < n) such that n mod k is even
 * Range:     u8 n <= 389, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t count = 0;
    if (n <= 2) return 0;
    for (idx_t k = 2; k < n; ++k) {
        if (((n % k) & 1) == 0) {
            ++count;
        }
    }
    return count;
}
