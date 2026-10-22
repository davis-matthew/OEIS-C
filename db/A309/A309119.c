/*
 * A309119  a(n) is the number of 1's minus the number of 2's among the
 *          ternary representations of the integers in the interval [0..n].
 * https://oeis.org/A309119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = Sum_{k=0..n} (A062756(k) - A081603(k)) where A062756(k) =
 *            # of 1's in the ternary expansion of k A081603(k) = # of 2's in
 *            the ternary expansion of k
 * Range:     u8 n <= 441, u16 n <= 6560, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        idx_t v = i;
        seq_t diff = 0;
        while (v > 0) {
            uint64_t d = v % 3;
            if (d == 1)      diff += 1;
            else if (d == 2) diff -= 1;
            v /= 3;
        }
        sum += diff;
    }
    return sum;
}
