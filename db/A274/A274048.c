/*
 * A274048  a(n) = A116640(A018900(n)) = A116623(A059893(A018900(n))).
 * https://oeis.org/A274048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = 2^(Row(n)+1) + 3*(3+2^Col(n))
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A274048 for any n ≥ 1. */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≥ n, with 1-based row index. */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) {
        ++r;
    }
    /* Column c is the position within the row (1‑based). */
    idx_t c = n - (r * (r - 1)) / 2;

    /* a(n) = 2^(r+1) + 3*(3 + 2^c) */
    seq_t part1 = pow2(r + 1);            /* 2^(Row+1)   */
    seq_t part2 = 3 * (3 + pow2(c));      /* 3*(3 + 2^Col) */
    return part1 + part2;
}
