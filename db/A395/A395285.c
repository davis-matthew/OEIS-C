/*
 * A395285  a(n) is the apex of the following triangle: start with the binary
 *          expansion of n, say (1, b_2, ..., b_w), as the first row; for k =
 *          2..w, the k-th row is obtained by performing between adjacent
 *          terms of the prior row the AND operation when b_k = 0, the OR
 *          operation when b_k = 1; a(0) = 0.
 * https://oeis.org/A395285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements OEIS A395285:
   a(n) is the apex of the triangle built from the binary representation of n.
   The first row is the bits of n (most significant first).
   For each following bit b_k (k ≥ 2) replace adjacent entries by
     – AND if b_k = 0
     – OR  if b_k = 1.
   a(0) is defined to be 0.  The function computes this directly.

   Complexity: O((#bits)^2) ≤ O(64²) operations for 64‑bit n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case for n = 0. */
    if (n == 0)
        return 0;

    /* Determine the number of bits in n. */
    idx_t w = 0;
    idx_t t = n;
    while (t != 0) {
        ++w;
        t >>= 1U;
    }

    /* Bits of n from most significant to least significant. */
    unsigned char bits[64];
    for (idx_t i = 0; i < w; ++i) {
        idx_t shift = w - 1 - i;
        bits[i] = (unsigned char)((n >> shift) & 1ULL);
    }

    /* Current row of the triangle; start with the bits. */
    seq_t row[64];
    for (idx_t i = 0; i < w; ++i)
        row[i] = (seq_t)bits[i];

    /* Build the triangle row by row. */
    idx_t len = w;
    for (idx_t k = 1; k < w; ++k) {          /* k = 1 corresponds to b_2 in the OEIS notation */
        unsigned char op = bits[k];
        for (idx_t i = 0; i < len - 1; ++i) {
            seq_t a1 = row[i];
            seq_t a2 = row[i + 1];
            row[i] = (op == 0) ? (a1 & a2) : (a1 | a2);
        }
        --len;
    }

    return row[0];
}
