/*
 * A330959  For any n >= 0: consider all pairs of numbers (x, y) whose binary
 *          representations can be interleaved (or shuffled) to produce the
 *          binary representation of n (possibly with leading zeros); a(n) is
 *          the least possible value of x XOR y (where XOR denotes the bitwise
 *          XOR operator).
 * https://oeis.org/A330959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = least possible value of x XOR y over all pairs (x, y)
 *            whose binary representations can be interleaved to produce the
 *            binary representation of n (allowing leading zeros)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static unsigned seq_bitlen(seq_t x)
{
    unsigned len = 0;
    while (x) {
        x >>= 1;
        ++len;
    }
    return len ? len : 1;  /* bit length of 0 is 1 */
}

seq_t a(idx_t n)
{
    unsigned L = seq_bitlen((seq_t)n);        /* length of binary representation */
    /* If length is 0 (should not happen) return 0 */
    if (L == 0) return 0;

    /* Enumerate all ways to split the L bits between x and y. */
    idx_t max_mask = (L >= 63) ? (idx_t)(-1) : ((1ULL << L) - 1);
    seq_t best = 0;
    bool first = true;

    for (idx_t mask = 0; mask <= max_mask; ++mask) {
        seq_t x = 0, y = 0;
        for (unsigned i = 0; i < L; ++i) {
            seq_t bit = ((seq_t)n >> (L - 1 - i)) & 1;
            if (mask & (1ULL << i))
                x = (x << 1) | bit;
            else
                y = (y << 1) | bit;
        }
        seq_t val = x ^ y;
        if (first || val < best) {
            best = val;
            first = false;
            if (best == 0) break;           /* cannot get better than 0 */
        }
    }
    return best;
}
