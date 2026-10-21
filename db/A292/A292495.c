/*
 * A292495  Triangle read by rows: T(n,k) = (-2)*T(n-1,k-1) + T(n,k-1) with
 *          T(2*m,0) = 0 and T(2*m+1,0) = (-1)^m.
 * https://oeis.org/A292495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, look
 * Formula:   T(n,k) = (-2)*T(n-1,k-1) + T(n,k-1) with T(2*m,0) = 0 and
 *            T(2*m+1,0) = (-1)^m. The sequence a(n) is the triangle read by
 *            rows: a(n) = T(r,k), where r and k are obtained from the index n
 *            by r*(r+1)/2 <= n < (r+1)*(r+2)/2 and k = n - r*(r+1)/2
 * Range:     i8 n <= 42, i16 n <= 117, i32 n <= 432, i64 n <= 1650,
 *            i128 n <= 6214, big any n
 * Helpers:   zeroth
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/zeroth.h"

/* Compute a(n) for the sequence A292495. */
seq_t a(idx_t n)
{
    /* Find row r and column k from the index n. */
    idx_t r = 0;
    for (;;) {
        /* next = (r+1)*(r+2)/2  is the index after the last element of row r+1. */
        idx_t next = ((r + 1) * (r + 2)) / 2;
        if (n < next)
            break;
        ++r;
    }
    idx_t k = n - (r * (r + 1)) / 2;

    /* Compute T(r,k) using the combinatorial formula:
       T(r,k) = sum_{j=0..k} binom(k,j) * (-2)^j * T(r-j,0). */
    seq_t sum = (seq_t)0;
    seq_t binom = (seq_t)1;      /* binom(k,0) */
    seq_t pow_mul = (seq_t)1;    /* (-2)^0 */

    for (idx_t j = 0; j <= k; ++j) {
        idx_t m = r - j;                     /* row index in the zeroth column */
        seq_t v = zeroth(m);                 /* T(m,0) */
        if (v != (seq_t)0) {
            sum += binom * pow_mul * v;
        }
        if (j < k) {
            /* Update binomial coefficient: binom(k,j+1) = binom(k,j) * (k-j)/(j+1) */
            binom = (binom * (seq_t)(k - j)) / (seq_t)(j + 1);
            /* Update power of -2 */
            pow_mul = pow_mul * (seq_t)-2;
        }
    }
    return sum;
}
