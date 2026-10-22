/*
 * A380856  In the binary expansion of n, arrange bits row-wise in a binary
 *          tree which is complete except for the last row and then read those
 *          bits in pre-order.
 * https://oeis.org/A380856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 9345, u32 n <= 9345, u64 n <= 9345,
 *            u128 n <= 9345, big any n
 * Helpers:   preorder_rec
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A380856.  For each integer n, interpret its binary representation,
   place the bits row‑wise into a complete binary tree (missing nodes
   only possible in the last row), then read the bits in pre‑order
   traversal.  The resulting bits form the output binary number. */
#include <stdint.h>
#include "../utils/preorder_rec.h"

static int bitlen(idx_t n) {
    /* Number of bits needed to represent n (at least 1 for n==0) */
    if (n == 0) return 1;
    int len = 0;
    idx_t m = n;
    while (m) {
        m >>= 1;
        len++;
    }
    return len;
}

seq_t a(idx_t n) {
    int L = bitlen(n);
    seq_t result = 0;
    preorder_rec(0, L, n, &result);
    return result;
}
