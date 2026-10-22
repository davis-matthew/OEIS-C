/*
 * A385848  Numbers not of the form 2^m*k + m + 1, for integers m, k >= 1.
 * https://oeis.org/A385848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 5504, u32 n <= 5504, u64 n <= 5504,
 *            u128 n <= 5504, big any n
 * Helpers:   is_representable
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = nth positive integer not representable as 2^m*k + m + 1 for m,k>=1 */
#include <stdbool.h>
#include "../utils/is_representable.h"

seq_t a(idx_t n) {
    // Compute the nth term (n >= 1).
    idx_t count = 0;
    seq_t N = (seq_t)1;
    while (count < n) {
        if (!is_representable(N)) {
            ++count;
            if (count == n) return N;
        }
        ++N;
    }
    return (seq_t)0;  // should never reach here for valid input
}
