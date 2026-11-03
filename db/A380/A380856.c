/*
 * A380856  In the binary expansion of n, arrange bits row-wise in a binary
 *          tree which is complete except for the last row and then read those
 *          bits in pre-order.
 * https://oeis.org/A380856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 32768, u32 n <= 32768, u64 n <= 32768,
 *            u128 n <= 32768, big any n
 * Validated: standard, pass, 85 terms through n = 9345 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A380856.  For each integer n, interpret its binary representation,
   place the bits row‑wise into a complete binary tree (missing nodes
   only possible in the last row), then read the bits in pre‑order
   traversal.  The resulting bits form the output binary number. */
#include <stdint.h>

static void preorder_rec(idx_t i, int L, idx_t n, seq_t *res) {
    if (i >= (idx_t)L) return;               /* node does not exist */
    seq_t bit = (seq_t)((n >> (L - 1 - i)) & 1);
    *res = (*res << 1) | bit;                /* append bit to result */
    preorder_rec(2 * i + 1, L, n, res);      /* left child   */
    preorder_rec(2 * i + 2, L, n, res);      /* right child  */
}

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
