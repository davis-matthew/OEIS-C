/*
 * A327191  For any n >= 0: consider the different ways to split the binary
 *          representation of n into two (possibly empty) parts, say with
 *          value x and y; a(n) is the least possible value of abs(x - y).
 * https://oeis.org/A327191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = min_{0≤i≤bitlen(n)} | (n >> (bitlen(n)-i)) - (n &
 *            ((1ULL<<(bitlen(n)-i))-1)) | where bitlen(n) is the number of
 *            bits in the binary representation of n
 * Range:     u8 n <= 365, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n) {
    if (n == 0)
        return 0;

    seq_t N = (seq_t)n;
    unsigned k = bitlen_seq_t(N);      /* number of bits in n (k >= 1) */
    seq_t min_diff = N;                /* split with left part empty */

    /* iterate over all non‑trivial splits: i bits on the left (1 <= i < k) */
    for (unsigned i = 1; i < k; ++i) {
        unsigned shift = k - i;                   /* bits of the right part */
        seq_t x = N >> shift;                    /* left part */
        seq_t mask = ((seq_t)1 << shift) - 1;    /* lower shift bits */
        seq_t y = N & mask;                      /* right part */
        seq_t diff = (x > y) ? (x - y) : (y - x);
        if (diff < min_diff)
            min_diff = diff;
    }
    return min_diff;
}
