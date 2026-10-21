/*
 * A285723  Transpose of square array A285722.
 * https://oeis.org/A285723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) is obtained by reading the square array A285722 by *
 *            anti-diagonals in lexicographic order. * * Let s =
 *            ceil((1+sqrt(1+8*n))/2) // diagonal index (i+j = s) * Let prev =
 *            (s-2)(s-1)/2 // number of terms before diagonal s * Let i = n -
 *            prev // row index (1-based) * Let j = s - i // column ind...
 * Range:     u8 n <= 276, u16 n <= 7260, u32 n <= 7260, u64 n <= 7260,
 *            u128 n <= 7260, big any n
 * Helpers:   get_diagonal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_diagonal.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* 1. Determine diagonal s (s >= 2) */
    idx_t s = get_diagonal(n);

    /* 2. Number of terms before diagonal s */
    idx_t prev = ((unsigned __int128)(s - 2) * (s - 1)) / 2;
    /* 3. Off‑by‑1 offset inside the diagonal (1 .. s-1) */
    idx_t offset = n - prev;           /* 1‑based */
    idx_t i = offset;                  /* row index   */
    idx_t j = s - offset;              /* column index */

    /* 4. Compute value of A285722(i,j) */
    if (i == j)
        return 0;

    if (j > i) {
        /* A285722(i,j) = ((j-2)*(j-1))/2 + (j-i) */
        seq_t m = (seq_t)(j - 2);
        seq_t val = m * ((seq_t)(j - 1)) / 2;
        return val + (seq_t)(j - i);
    } else { /* j < i */
        /* A285722(i,j) = ((i-2)*(i-1))/2 + j */
        seq_t m = (seq_t)(i - 2);
        seq_t val = m * ((seq_t)(i - 1)) / 2;
        return val + (seq_t)j;
    }
}
