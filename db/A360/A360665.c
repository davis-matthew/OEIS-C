/*
 * A360665  Square array T(n, k) = k*((2*n-1)*k+1)/2 read by rising
 *          antidiagonals.
 * https://oeis.org/A360665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = T(n_row, k) where T(n,k) = k*((2*n-1)*k+1)/2, and the
 *            square array T is read by rising antidiagonals: list entries
 *            with n+k = d for d = 0,1,2,…, each antidiagonal enumerated with
 *            n decreasing from d to 0
 * Range:     i8 n <= 60, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Find the antidiagonal d such that the first (d+1) entries of
       this antidiagonal are positioned after n.  prev holds the
       number of entries before this antidiagonal. */
    idx_t prev = 0;   /* total entries up to antidiagonal d-1 */
    idx_t d = 0;      /* current antidiagonal index */

    while (1) {
        idx_t next = prev + (d + 1);   /* entries up to antidiagonal d */
        if (next > n) break;          /* d is the correct antidiagonal */
        prev = next;
        ++d;
    }

    idx_t offset = n - prev;          /* offset within antidiagonal d */
    idx_t n_row = d - offset;         /* n in T(n,k) */
    idx_t k = offset;                 /* k in T(n,k) */

    seq_t n_row_s = (seq_t)n_row;
    seq_t k_s     = (seq_t)k;
    seq_t val = k_s * ( ((seq_t)2 * n_row_s - (seq_t)1) * k_s + (seq_t)1 ) / (seq_t)2;

    return val;
}
