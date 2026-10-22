/*
 * A352517  Number of weak excedances (parts on or above the diagonal) of the
 *          n-th composition in standard order.
 * https://oeis.org/A352517
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of indices i for which the i‑th part of the n‑th
 *            composition (in standard order) satisfies part_i ≥ i. The n‑th
 *            composition is obtained by: 1. Convert n to binary and reverse
 *            the bit string. 2. Let p[0] = 0, and for each ‘1’ at position j
 *            (counting from 1 starting with the least sig...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* positions of set bits plus a leading zero */
    const size_t MAX_BITS = 64;        /* fits uint64_t */
    size_t pos[65];                   /* pos[0] = 0, then positions of 1‑bits */
    size_t poslen = 1;
    pos[0] = 0;

    idx_t temp = n;
    size_t bitpos = 1;                /* 1‑based position in reversed binary */

    while (temp != 0) {
        if (temp & 1)
            pos[poslen++] = bitpos;
        temp >>= 1;
        ++bitpos;
    }

    /* count weak excedances */
    size_t partslen = poslen - 1;      /* number of parts */
    seq_t count = 0;

    /* iterate over parts in reversed order */
    for (size_t i = 1; i <= partslen; ++i) {
        size_t j = partslen - i;      /* index in forward differences */
        seq_t part = (seq_t)(pos[j + 1] - pos[j]);  /* part size */
        if (part >= (seq_t)i)
            ++count;
    }

    return count;
}
