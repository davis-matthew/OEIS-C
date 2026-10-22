/*
 * A393140  Triangle read by rows. T(n, k) = C(n, k)*C(k,
 *          floor(k/2))*Sum_{i=0..k} C(k, i)*C(n-i, floor((n-i)/2)).
 * https://oeis.org/A393140
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, walk
 * Formula:   a(n) is obtained by flattening the triangle T(n,k) where T(n,k)
 *            = C(n,k) * C(k,floor(k/2)) * Σ_{i=0..k} C(k,i) *
 *            C(n-i,floor((n-i)/2)). The index n is interpreted as the
 *            position in row‑major order starting with row 0
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 98, u64 n <= 315,
 *            u128 n <= 1111, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row (n) and column (k) in the triangle that
       corresponds to the flat index n. */
    idx_t low = 0;
    idx_t high = (n == 0 ? 1 : n * 2);  /* upper bound for search */

    while (low < high) {
        idx_t mid = (low + high) / 2;
        /* total number of terms up to and including row mid */
        idx_t total = (mid + 1) * (mid + 2) / 2;
        if (total <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t row = low;                           /* 0‑based row number */
    idx_t first_of_row = row * (row + 1) / 2;   /* first index of this row */
    idx_t col = n - first_of_row;              /* 0‑based column (k) */

    /* Compute T(row, col) using the explicit formula. */
    idx_t k_half = col / 2;

    seq_t Cnk = binomial(row, col);            /* C(n,k) */
    seq_t CkHalf = binomial(col, k_half);      /* C(k,floor(k/2)) */

    seq_t sum = 0;
    for (idx_t i = 0; i <= col; ++i) {
        seq_t Cki = binomial(col, i);
        idx_t rem = row - i;
        idx_t rem_half = rem / 2;
        seq_t Crem = binomial(rem, rem_half);
        sum += Cki * Crem;
    }

    return Cnk * CkHalf * sum;
}
