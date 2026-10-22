/*
 * A322630  Arithmetic table similar to multiplication with different rules
 *          for odd and even products, read by antidiagonals. T(n,k) = (n*k +
 *          A319929(n,k))/2.
 * https://oeis.org/A322630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the entry read by antidiagonals of the arithmetic table
 *            T(i,j) defined by T(i,j) = (i*j + i + j - 1)/2 if i,j are odd;
 *            T(i,j) = (i*j + i)/2 if i is even, j is odd; T(i,j) = (i*j +
 *            j)/2 if i is odd, j is even; T(i,j) = (i*j)/2 if i,j are even.
 *            The pair (i,j) for index n satisfies i+j = D w...
 * Range:     u8 n <= 920, u16 n <= 9928, u32 n <= 9928, u64 n <= 9928,
 *            u128 n <= 9928, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    /* Find diagonal D: smallest integer s.t. (D-1)*D/2 >= n */
    idx_t D = 2;                          /* minimum possible value */
    while ( (D-1) * D / 2 < n ) ++D;

    /* Number of terms before this diagonal */
    idx_t prev = (D-2) * (D-1) / 2;

    /* Position inside the diagonal: i = offset, j = D - i */
    idx_t i = n - prev;                   /* 1‑based index inside diagonal */
    idx_t j = D - i;

    /* Compute T(i,j) using the piecewise definition */
    seq_t ii = (seq_t)i;
    seq_t jj = (seq_t)j;
    seq_t prod = ii * jj;
    seq_t result;

    if (i & 1ULL) {                       /* i is odd */
        if (j & 1ULL) {                   /* j is odd */
            result = (prod + ii + jj - 1) / 2;
        } else {                          /* j is even */
            result = (prod + jj) / 2;
        }
    } else {                              /* i is even */
        if (j & 1ULL) {                   /* j is odd */
            result = (prod + ii) / 2;
        } else {                          /* j is even */
            result = prod / 2;
        }
    }

    return result;
}
