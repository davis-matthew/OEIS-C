/*
 * A327194  For any n >= 0: consider the different ways to split the binary
 *          representation of n into two (possibly empty) parts, say with
 *          value x and y; a(n) is the least possible value of x^2 + y^2.
 * https://oeis.org/A327194
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) = min_{0 ≤ k ≤ bitlen(n)} [ x^2 + y^2 ] where * x is the
 *            upper k bits of n and y is the remaining bits. For n = 0, * a(0)
 *            = 0
 * Range:     u8 n <= 15, u16 n <= 1147, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* special case */
    seq_t sn = (seq_t)n;                 /* convert n to seq_t */
    unsigned len = bitlen_seq_t(sn);     /* number of significant bits */

    /* Initially consider the split where y = 0, x = n. */
    seq_t best = sn * sn;

    /* Loop over all non‑trivial splits: 1 ≤ k < len. */
    for (unsigned k = 1; k < len; ++k) {
        unsigned shift = len - k;                 /* number of bits in y */
        seq_t x, y;

        /* Compute x: upper k bits.  When shift == 0, x = sn. */
        if (shift == 0)
            x = sn;
        else
            x = sn >> shift;

        /* Compute y: lower shift bits.
           When shift == 0, y = 0. */
        if (shift == 0)
            y = 0;
        else
            y = sn & (((seq_t)1 << shift) - 1);

        seq_t val = x * x + y * y;
        if (val < best) best = val;
    }

    /* The case k = 0 (x = 0, y = n) gives the same value as k = len,
       so ``best`` already contains the global minimum. */
    return best;
}
