/*
 * A261721  Fourth-dimensional figurate numbers.
 * https://oeis.org/A261721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(offset+3,3) + r * binomial(offset+3,4) * where
 *            offset = n - (k-1)k/2 - 1, k = min{m: m(m+1)/2 >= n}, * r = k -
 *            offset (r is the row index in the fourth‑dimensional * figurate
 *            array A(r,offset) = binomial(offset+3,3)+ r*binomial(offset+3,4)
 *            )
 * Range:     u8 n <= 33, u16 n <= 372, u32 n <= 10011, u64 n <= 10011,
 *            u128 n <= 10011, big any n
 * Validated: standard, pass, 83 terms through n = 9613 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"

static inline idx_t min_k_triangular(idx_t n)
{
    /* Find smallest k such that k*(k+1)/2 >= n.  k>=1. */
    idx_t k = 0;
    idx_t tri = 0;
    while (tri < n) {
        ++k;
        tri += k;
    }
    return k;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* offset is 1, undefined for n<1 */

    /* Determine diagonal index k and offset within that diagonal. */
    idx_t k = min_k_triangular(n);           /* k = diagonal number + 1 */
    idx_t tri = k * (k + 1) / 2;              /* triangular number of k */
    idx_t prev_tri = tri - k;                /* triangular number of k-1 */
    idx_t offset = n - prev_tri - 1;         /* 0‑based position in the diagonal */
    idx_t r = k - offset;                    /* row number in the array (1‑based) */

    /* Value from the fourth‑dimensional figurate array. */
    seq_t part1 = binomial(offset + 3, 3);
    seq_t part2 = binomial(offset + 3, 4);
    return part1 + (seq_t)r * part2;
}
