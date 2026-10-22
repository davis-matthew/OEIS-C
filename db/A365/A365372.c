/*
 * A365372  Array read by ascending antidiagonals: A(n, k) = n*(k*n^2 - 1)
 *          with k > 0.
 * https://oeis.org/A365372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   A(n, k) = n * (k * n^2 - 1) (n > 0, k > 0)
 * Range:     u8 n <= 21, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) reads the array A(n, k) by ascending antidiagonals
   (sum n+k) and within each antidiagonal from larger n to smaller n.
   a(1) = A(1,1) = 0, a(2) = A(2,1) = 6, a(3) = A(1,2) = 1, ... */

#include <stdint.h>  /* for uint64_t (idx_t) */

static idx_t triangular_inverse(idx_t n) {
    /* Return the smallest m ≥ 1 such that m*(m+1)/2 ≥ n. */
    idx_t low = 1;
    idx_t high = 1;
    seq_t n_big = (seq_t)n;

    /* Increase high until the triangular number is large enough. */
    while (((seq_t)high * ((seq_t)high + 1)) / 2 < n_big) {
        high <<= 1;          /* safe, high ≤ 2^64-1 */
    }

    /* Binary search for the smallest suitable m. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t val = ((seq_t)mid * ((seq_t)mid + 1)) / 2;
        if (val >= n_big) high = mid;
        else low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* Locate the antidiagonal and coordinates (row, col). */
    idx_t m = triangular_inverse(n);                 /* diagonal index (1‑based) */
    seq_t prev_tri = ((seq_t)(m - 1) * (seq_t)m) / 2;  /* triangular number before this diagonal */
    seq_t pos_big  = (seq_t)n - prev_tri;             /* position in the diagonal (1‑based) */
    idx_t pos = (idx_t)pos_big;                       /* fits in idx_t */
    idx_t sumS = m + 1;                               /* n + k for this diagonal */
    idx_t row = sumS - pos;                           /* larger n first */
    idx_t col = pos;                                  /* smaller k last */

    /* Compute A(row, col) = row * (col * row^2 - 1). */
    seq_t row_s   = (seq_t)row;
    seq_t row_sq  = row_s * row_s;
    seq_t val = row_s * (((seq_t)col * row_sq) - (seq_t)1);
    return val;
}
