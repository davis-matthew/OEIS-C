/*
 * A310342  Coordination sequence Gal.5.253.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, for n > 0: a(n) = 4 + floor((n-1)/5)*22 +
 *            prefix[(n-1)%5], where prefix = {0, 6, 8, 14, 18, 22} represents
 *            the partial sums of the periodic increment pattern (6,2,6,4,4)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t prefix[6] = {
    (seq_t)0, (seq_t)6, (seq_t)8, (seq_t)14, (seq_t)18, (seq_t)22
};

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    idx_t m = n - 1;
    seq_t cycles = (seq_t)(m / 5) * (seq_t)22;
    seq_t rem = (seq_t)(m % 5);
    return (seq_t)4 + cycles + prefix[(size_t)rem];
}
