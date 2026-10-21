/*
 * A269752  Table of inverse permutations of the rows of A131987: Position of
 *          numbers inserted in "storage order" into a perfect binary table of
 *          2^k-1 nodes.
 * https://oeis.org/A269752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) gives the storage order index of the n‑th number * inserted
 *            into a perfect binary tree of size 2^k–1 in breadth‑first *
 *            order. The tree nodes are numbered in the usual level‑order
 *            array * representation (indices 1…2^k–1). Row r contains 2^r–1
 *            terms. * The algorithm finds the row r containing...
 * Range:     u8 n <= 73, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor(log₂(v)) for v > 0 */
static inline int floor_log2_u64(idx_t v) {
    int r = 0;
    while (v >> 1) { ++r; v >>= 1; }
    return r;
}

seq_t a(idx_t n) {
    /* Locate the row r that contains n */
    idx_t start = 1;
    int r = 1;
    for (;;) {
        unsigned __int128 len128 = ((unsigned __int128)1 << r) - 1;
        idx_t len = (idx_t)len128;
        idx_t end = start + len - 1;
        if (n <= end) break;
        start = end + 1;
        ++r;
    }

    /* 0‑based position within the row */
    idx_t pos = n - start;

    /* Compute depth and offset */
    int d = floor_log2_u64(pos + 1);
    unsigned __int128 offset128 = (unsigned __int128)pos -
                                 ((((unsigned __int128)1 << d) - 1));

    /* Levels are 0‑based; compute half and step using 128‑bit to avoid overflow */
    unsigned __int128 half  = ((unsigned __int128)1 << (r - 1 - d));
    unsigned __int128 step  = ((unsigned __int128)1 << (r - d));
    unsigned __int128 idx128 = half + offset128 * step;

    return (seq_t)idx128;
}
