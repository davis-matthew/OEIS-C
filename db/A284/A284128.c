/*
 * A284128  Hosoya triangle of Fermat Lucas type, read by rows.
 * https://oeis.org/A284128
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (2^k + 1)*(2^(r-k+1)+1) where r is the row index
 *            (starting at 1) that contains the term, k is the column index (1
 *            ≤ k ≤ r), and the mapping from n to (r, k) is: i = n - 8 //
 *            1-based position in the flattened triangle r =
 *            ceil((sqrt(8*i+1)-1)/2) k = i - r*(r-1)/2 // position within the
 *            row
 * Range:     u8 n <= 29, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   _floor_isqrt_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/_floor_isqrt_u64.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    if (n < 9) return 0;           /* behavior below the offset is irrelevant */

    idx_t i = n - 8;               /* position in flattened triangle */

    /* Find the row r containing the position i. */
    idx_t sq = _floor_isqrt_u64(8 * i + 1);
    idx_t r = (sq - 1) / 2;
    if (r * (r + 1) / 2 < i) r++;  /* adjust if we fell short */

    /* Column index within the row. */
    idx_t k = i - r * (r - 1) / 2;

    /* Compute (2^k + 1) * (2^(r-k+1) + 1). */
    seq_t term1 = pow2(k);
    term1 += 1;

    idx_t exp2 = r - k + 1;
    seq_t term2 = pow2(exp2);
    term2 += 1;

    return term1 * term2;
}
