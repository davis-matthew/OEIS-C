/*
 * A352514  Number of strong nonexcedances (parts below the diagonal) of the
 *          n-th composition in standard order.
 * https://oeis.org/A352514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of parts in the n‑th composition in standard order
 *            that are strictly less than their 1‑based index. The composition
 *            is obtained by taking the positions of 1–bits in the binary
 *            representation of n (counting from the least significant bit) as
 *            a set of increasing integers, prepending 0...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* collect the 1‑bit positions of n (1‑based from LSB) */
    idx_t pos[64];
    int r = 0;
    idx_t bits = n;
    idx_t bitpos = 1;
    while (bits != 0) {
        if (bits & 1ULL)
            pos[r++] = bitpos;
        bits >>= 1;
        bitpos++;
    }

    /* count strong nonexcedances: parts < their 1‑based index */
    seq_t cnt = 0;
    for (int k = 0; k < r; ++k) {
        idx_t part = pos[r - 1 - k]
                   - ((k == r - 1) ? 0 : pos[r - 2 - k]);
        idx_t i = k + 1;          /* 1‑based position in the composition */
        if (part < i)
            ++cnt;
    }
    return cnt;
}
