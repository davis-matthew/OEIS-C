/*
 * A317495  Triangle read by rows: T(0,0) = 1; T(n,k) =2 * T(n-1,k) +
 *          T(n-3,k-1) for k = 0..floor(n/3); T(n,k)=0 for n or k < 0.
 * https://oeis.org/A317495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 50, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A317495:  Triangle read by rows.
   Definition of the triangle:
     T(0,0) = 1;
     T(n,k) = 2 * T(n-1,k) + T(n-3,k-1)   for 0 ≤ k ≤ floor(n/3);
     T(n,k) = 0 for n<0 or k<0.
   The output array is flattened row‑major (k increases fastest).
   a(n) = T(row, col) where (row,col) are obtained from n by
   searching for the first row with cumulative length > n. */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

static inline idx_t row_len(idx_t n)
{
    /* number of entries in row n: k = 0..⌊n/3⌋ */
    return n / 3 + 1;
}

seq_t a(idx_t n)
{
    idx_t row = 0;
    idx_t pos = 0;          /* number of entries before current row */

    /* find row such that pos + row_len(row) > n */
    while (pos + row_len(row) <= n) {
        pos += row_len(row);
        ++row;
    }

    idx_t col = n - pos;    /* index within the row */

    /* compute T(row,col) = binom(row-2*col, col) * 2^(row-3*col) */
    seq_t b = binomial(row - 2 * col, col);
    seq_t p = pow2(row - 3 * col);
    return b * p;
}
