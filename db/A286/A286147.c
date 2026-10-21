/*
 * A286147  Square array read by antidiagonals: A(n,k) = T(n XOR k, n), where
 *          T(n,k) is sequence A001477 considered as a two-dimensional table,
 *          and XOR is bitwise-xor (A003987).
 * https://oeis.org/A286147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (( (n XOR k) + n ) * ((n XOR k) + n + 1))/2 + (n XOR k),
 *            where k is the column index obtained when the index n is read by
 *            descending antidiagonals: (0,0),(0,1),(1,0),(0,2),(1,1),(2,0),
 * Range:     u8 n <= 76, u16 n <= 9932, u32 n <= 9932, u64 n <= 9932,
 *            u128 n <= 9932, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* compute antidiagonal coordinates (i,j) from the flat index n.
   The enumeration follows the order: (0,0), (0,1), (1,0), (0,2), (1,1), (2,0), ... */
static void antidiag_coords(idx_t n, idx_t *i, idx_t *j)
{
    idx_t cum = 0;   /* number of elements before the current diagonal */
    idx_t d   = 0;   /* diagonal index (starting from 0) */

    /* Find the diagonal d such that cum + (d+1) > n */
    while (1) {
        idx_t len = d + 1;           /* size of diagonal d */
        if (n < cum + len)
            break;
        cum += len;
        ++d;
    }

    *i = n - cum;        /* row index within the diagonal */
    *j = d - *i;         /* column index = diagonal index - row index */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiag_coords(n, &i, &j);

    idx_t xor_val = i ^ j;    /* n XOR k */

    /* all arithmetic is performed in seq_t to avoid overflow */
    seq_t s = (seq_t)(xor_val + i);
    seq_t res = s * (s + 1) / 2 + (seq_t)xor_val;
    return res;
}
