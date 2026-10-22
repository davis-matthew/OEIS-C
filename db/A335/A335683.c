/*
 * A335683  Array read by antidiagonals: T(n,k) (n >= 1, k >= 1) =
 *          Sum_{i=1..n, j=1..k, gcd(i,j)=2} (n+1-i)*(k+1-j).
 * https://oeis.org/A335683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A335683: Array read by antidiagonals.
   Formula: T(n,k) = Σ_{1≤i≤n, 1≤j≤k, gcd(i,j)=2} (n+1-i)(k+1-j);
   a(n) = T(i,j) where (i,j) is the n‑th element of the array
   read along successive antidiagonals (row   1 column 1, row 1 column 2,
   row 2 column 1, row 1 column 3, row 2 column 2, row 3 column 1, …). */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b != 0) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

/* Computes the n-th element of A335683. */
seq_t a(idx_t n) {
    /* Find diagonal d such that d*(d+1)/2 >= n. */
    idx_t d = 1;
    while (d * (d + 1) / 2 < n) {
        ++d;
    }

    /* Offset within the diagonal (0‑based). */
    idx_t previous = (d - 1) * d / 2;
    idx_t offset = n - previous - 1;

    /* Coordinates (i,j) in the array.  Rows increase with offset. */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* Only even i',j' contribute.  Let i' = 2x, j' = 2y with gcd(x,y)=1. */
    idx_t i_half = i / 2;
    idx_t j_half = j / 2;

    seq_t sum = 0;

    for (idx_t x = 1; x <= i_half; ++x) {
        seq_t term_i = (seq_t)(i + 1 - (seq_t)2 * (seq_t)x);   /* (i+1-2x) in seq_t */
        for (idx_t y = 1; y <= j_half; ++y) {
            if (gcd_idx(x, y) == 1) {
                seq_t term_j = (seq_t)(j + 1 - (seq_t)2 * (seq_t)y); /* (j+1-2y) */
                sum += term_i * term_j;
            }
        }
    }

    return sum;
}
