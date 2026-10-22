/*
 * A330408  Table of A(n,k) read by antidiagonals, where A(1,k)=k; A(n,k) is
 *          the least multiple of n >= A(n-1,k).
 * https://oeis.org/A330408
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   _ceil_div, _diag_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A330408:
 *   A(1,k) = k.
 *   A(n,k) = least multiple of n that is ≥ A(n-1,k).
 *   a(n) reads the table A(n,k) along antidiagonals.
 * This implementation uses a direct calculation for A(n,k) based on
 * i = row index, j = column index derived from the antidiagonal
 * position of n.  No storage of the full table is required.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/_ceil_div.h"
#include "../utils/_diag_index.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;  /* undefined for n < 1 */

    /* Determine row (i) and column (j) from diagonal index. */
    idx_t d = _diag_index(n);  /* diagonal number (1‑based) */
    idx_t prev = (__int128)(d - 1) * d / 2;
    idx_t pos = n - prev;          /* 1‑based position within the diagonal */
    idx_t i = pos;                 /* row index */
    idx_t j = d - pos + 1;         /* column index */

    /* Compute A(i,j) by iterating m = 2..i. */
    seq_t value = (seq_t)j;
    for (idx_t m = 2; m <= i; ++m) {
        seq_t q = value / (seq_t)m;
        if (value % (seq_t)m != 0)
            ++q;
        value = q * (seq_t)m;
    }
    return value;
}
