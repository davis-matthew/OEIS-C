/*
 * A320508  T(n,k) = binomial(n - k - 1, k), 0 <= k < n, and T(n,n) = (-1)^n,
 *          triangle read by rows.
 * https://oeis.org/A320508
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = T(row,col) * where row = floor((sqrt(8*n+1)-1)/2), * col
 *            = n - row(row+1)/2, * and T(row,col) = binomial(row-col-1, col)
 *            if col<row, * = (-1)^row if col==row
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static inline idx_t floor_sqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t diff = hi - lo;
        idx_t mid  = lo + (diff >> 1) + (diff & 1);  /* ceil midpoint */
        if (mid <= x / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* compute the row index */
    idx_t r = (floor_sqrt_u64((n * (idx_t)8U + 1U)) - 1U) / 2U;
    /* first index of this row */
    idx_t start = r * (r + 1U) / 2U;
    /* column within the row */
    idx_t k = n - start;

    if (k == r) {
        return (k & 1U) ? (seq_t)-1 : (seq_t)1;
    } else {
        return binomial(r - k - 1U, k);
    }
}
