/*
 * A292977  Square array A(n,k), n >= 0, k >= 0, read by antidiagonals, where
 *          column k is the expansion of e.g.f. exp(-k*x)/(1 - x).
 * https://oeis.org/A292977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is the nth element of the square array
 *            read by antidiagonals; T(i,j) = i! * SUM_{k=0..i} (-j)^k / k!
 * Range:     i8 n <= 26, i16 n <= 43, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/antidiagonal_coords.h"

seq_t a(idx_t n) {
    /* Find the (row,column) pair for this index */
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    /* Compute i! */
    seq_t term = 1;
    for (idx_t t = 1; t <= i; ++t) {
        term *= (seq_t)t;
    }

    /* Sum the series using the recurrence
       term_{k+1} = term_k * (-j) / (k+1)   (term_k = i! / k! * (-j)^k)
    */
    seq_t sum = term;          /* k = 0 term */
    seq_t negj = -(seq_t)j;    /* (-j) as seq_t */
    for (idx_t k = 0; k < i; ++k) {
        term = (term * negj) / (seq_t)(k + 1);
        sum += term;
    }
    return sum;
}
