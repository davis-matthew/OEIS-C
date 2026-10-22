/*
 * A334593  Number of ones in XOR-triangle with first row generated from the
 *          binary expansion of n.
 * https://oeis.org/A334593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 1’s in the XOR‑triangle whose top row consists
 *            of the binary expansion of n
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t len = floor_log2_uint64(n) + 1;        /* number of bits in n */

    /* store the current row of the triangle (max 64 bits for 64‑bit n) */
    unsigned char row[64];
    unsigned char next_row[64];

    /* initialise the top row from the binary digits of n */
    for (idx_t i = 0; i < len; ++i)
        row[i] = (n >> (len - 1 - i)) & 1;

    seq_t total = 0;

    /* count ones in the top row */
    for (idx_t i = 0; i < len; ++i)
        total += row[i];

    /* generate subsequent rows */
    idx_t m = len;
    while (m > 1) {
        /* compute the next row by XOR of adjacent elements */
        for (idx_t i = 0; i < m - 1; ++i)
            next_row[i] = row[i] ^ row[i + 1];
        m = m - 1;   /* new length */

        /* count ones in the new row */
        for (idx_t i = 0; i < m; ++i)
            total += next_row[i];

        /* copy the next row back into row for the next iteration */
        for (idx_t i = 0; i < m; ++i)
            row[i] = next_row[i];
    }

    return total;
}
