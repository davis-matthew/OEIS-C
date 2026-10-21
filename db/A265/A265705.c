/*
 * A265705  Triangle read by rows: T(n,k) = k IMPL n, 0 <= k <= n, bitwise
 *          logical IMPL.
 * https://oeis.org/A265705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, look
 * Formula:   a(n) = ((~col) | row) & ((1 << bitlen(row)) - 1) where row =
 *            ⌊(√(8n+1)–1)/2⌋, col = n – row(row+1)/2, and bitlen(row) = 1 if
 *            row==0 else ⌊log₂(row)⌋+1. This reads the triangle T(n,k)=k IMPL
 *            n (bitwise implication) row‑by‑row
 * Range:     u8 n <= 8945, u16 n <= 8945, u32 n <= 8945, u64 n <= 8945,
 *            u128 n <= 8945, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 explicitly. */
    if (n == 0)
        return 0;

    /* Find the row containing the n‑th element. */
    idx_t row = (idx_t)((sqrt((double)(8.0 * n + 1)) - 1.0) / 2.0);

    /* Number of elements preceding this row. */
    __int128 temp = (__int128)row * (row + 1) / 2;
    idx_t prev = (idx_t)temp;

    /* Column index within the row. */
    idx_t col = n - prev;

    /* Bit-width of the row number (n). */
    idx_t bitlen = (row == 0) ? 1 : (idx_t)floor_log2_uint64(row) + 1;

    /* Mask to keep only the lower bitlen bits. */
    seq_t mask = (((seq_t)1) << bitlen) - 1U;

    /* Compute the triangle entry. */
    seq_t val = ((~(seq_t)col) | (seq_t)row) & mask;
    return val;
}
