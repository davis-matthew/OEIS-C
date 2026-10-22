/*
 * A341091  Triangle read by rows: Coefficients for calculation of the sum of
 *          all the finite differences from order zero to order k.
 *          Sum_{n=0..k} T(n, k)*b(n) = b(0) + b(1) + ... + b(k) + (b(1) -
 *          b(0)) + ... + (b(k) - b(k-1)) + ((b(2) - b(1)) - (b(1) - b(0))) +
 *          ... .
 * https://oeis.org/A341091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) is the entry of the triangular array T(n,k) read by rows.
 *            T(n,k) = Σ_{m=n..k} (-1)^(m+n) · binomial(m+1,n) where k is the
 *            unique row with k(k+1)/2 ≤ n < (k+1)(k+2)/2 and the column index
 *            is i = n - k(k+1)/2
 * Range:     i8 n <= 30, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial, tri_leq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* * Utility header providing the binomial function   */
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/tri_leq.h"

/* Return the row index k such that k(k+1)/2 ≤ n < (k+1)(k+2)/2. */
static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = 1;
    while (tri_leq(high, n)) high <<= 1;          /* exponential search */
    while (low < high) {                         /* binary search   */
        idx_t mid = low + (high - low + 1) / 2;
        if (tri_leq(mid, n))
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the row k and column i for the flattened index n. */
    idx_t k = find_row(n);
    idx_t prev = k * (k + 1) / 2;   /* entries before row k        */
    idx_t i = n - prev;             /* column within the row      */

    /* Compute T(i,k) = Σ_{m=i..k} (-1)^(m+i) · binomial(m+1,i). */
    seq_t sum = 0;
    for (idx_t m = i; m <= k; ++m) {
        seq_t b = binomial(m + 1, i);
        if (((m + i) & 1) == 0)
            sum += b;
        else
            sum -= b;
    }
    return sum;
}
