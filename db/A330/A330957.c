/*
 * A330957  For any n >= 0: consider all pairs of numbers (x, y) whose binary
 *          representations can be interleaved (or shuffled) to produce the
 *          binary representation of n (possibly with leading zeros); a(n) is
 *          the least possible value of max(x, y).
 * https://oeis.org/A330957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = minimal possible max(x, y) over all interleavings of n’s
 *            binary representation into two subsequences. This implementation
 *            brute‑forces all assignments of the bits of n (which is at most
 *            a few dozen for the problem limits). It works for any
 *            seq_t/idx_t that support the allowed integer opera...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    /* number of bits in n */
    idx_t bitlen = 0;
    idx_t tmp = n;
    while (tmp) {
        ++bitlen;
        tmp >>= 1;
    }

    /* enumerate all subsets of the bit positions */
    idx_t limit = (idx_t)1 << bitlen;
    seq_t best = 0;
    bool first = true;

    for (idx_t mask = 0; mask < limit; ++mask) {
        seq_t x = 0, y = 0;
        for (idx_t i = 0; i < bitlen; ++i) {
            int bit = (int)((n >> (bitlen - 1 - i)) & 1u);
            if (mask & ((idx_t)1 << i)) {
                x = (x << 1) | (seq_t)bit;
            } else {
                y = (y << 1) | (seq_t)bit;
            }
        }
        seq_t m = x > y ? x : y;
        if (first || m < best) {
            best = m;
            first = false;
        }
    }

    return best;
}
