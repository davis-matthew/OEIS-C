/*
 * A360099  To get A(n,k), replace 0's in the binary expansion of n with (-1)
 *          and interpret the result in base k; square array A(n,k), n>=0,
 *          k>=0, read by antidiagonals.
 * https://oeis.org/A360099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, look, base
 * Formula:   a(n) = A(i, j) where i = n - d(d+1)/2, j = d - i, and A(0, j) =
 *            0, A(i, j) = j*A(i/2, j) + 2*(i%2) - 1 for i > 0. (i,k) run over
 *            non‑negative integers, d is the diagonal number: d = max { d |
 *            d(d+1)/2 <= n }
 * Range:     i8 n <= 112, i16 n <= 480, i32 n <= 5214, i64 n <= 20300,
 *            i128 n <= 20300, big any n
 * Validated: standard, pass, 107 terms through n = 9221 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursive computation of the array entry A(i, j) */
static seq_t A_val(idx_t i, seq_t j)
{
    if (i == 0)
        return 0;
    seq_t half = A_val(i >> 1, j);
    seq_t sign = (i & 1) ? 1 : -1;
    return j * half + sign;
}

/* Find diagonal number d from index n:
   d = largest integer with d(d+1)/2 <= n.
   Uses 128‑bit arithmetic to avoid overflow. */
static idx_t diagonal_from_n(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Find an upper bound for d: high*(high+1)/2 > n. */
    while (((__int128)high * (high + 1)) / 2 <= n)
        high <<= 1;
    /* Binary search in [low, high). */
    while (low + 1 < high) {
        idx_t mid = (low + high) >> 1;
        if (((__int128)mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    idx_t d = diagonal_from_n(n);
    idx_t offset = n - d * (d + 1) / 2;   /* offset within the diagonal */
    idx_t i = offset;
    idx_t j = d - offset;
    return A_val(i, (seq_t)j);
}
