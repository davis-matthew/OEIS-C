/*
 * A247188  a(0) = 0. a(n) is the number of repeating sums in the collection
 *          of all sums of any k elements in [a(0), ... a(n-1)] chosen without
 *          replacement for 2 <= k <= n.
 * https://oeis.org/A247188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - n - 1 - 2^(n-3) for n >= 3, a(0) = a(1) = a(2) = 0
 *            (computed using pow2 from the utils library)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 3u) return 0;
    seq_t two_n = pow2(n);
    seq_t two_n_minus3 = pow2(n - 3);
    seq_t res = two_n - (seq_t)n - 1 - two_n_minus3;
    return res;
}
