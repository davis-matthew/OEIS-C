/*
 * A327186  For any n >= 0: consider the different ways to split the binary
 *          representation of n into two (possibly empty) parts, say with
 *          value x and y; a(n) is the least possible value of x OR y (where
 *          OR denotes the bitwise OR operator).
 * https://oeis.org/A327186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   For n ≥ 0, split the binary representation of n into two parts,
 *            * with values x (left part) and y (right part). a(n) is the
 *            minimum * possible value of (x | y). For n = 0 the value is 0.
 *            The implementation * evaluates all splits O(log₂ n) times
 * Range:     u8 n <= 274, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n) {
    /* Special case: 0 → 0 */
    if (n == 0) return (seq_t)0;

    seq_t sn = (seq_t)n;
    unsigned k = bitlen_seq_t(sn);   /* number of bits in n */
    seq_t best = sn;                 /* extreme splits give value n */

    /* Loop through all internal split positions (1 .. k‑1) */
    for (unsigned i = 1; i < k; ++i) {
        unsigned m = k - i;           /* bits in the right part       */
        seq_t x = sn >> m;            /* upper i bits                 */
        seq_t mask;
        if (m == 0) {
            mask = (seq_t)0;
        } else {
            mask = ((seq_t)1 << m) - (seq_t)1;
        }
        seq_t y = sn & mask;          /* lower k‑i bits               */
        seq_t orv = x | y;
        if (orv < best) {
            best = orv;
            if (best == 1) break;  /* cannot get smaller than 1 */
        }
    }
    return best;
}
