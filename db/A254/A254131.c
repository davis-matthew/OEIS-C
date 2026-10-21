/*
 * A254131  Rectangular array A read by upward antidiagonals in which the
 *          entry in row n and column k is defined by A(n,k) = A254067(n,k) -
 *          A257499(n,k), n,k >= 1.
 * https://oeis.org/A254131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   A(n,k) = (1 + (3^n - 2^(n + 1))*(6*k - 3 + 2*(-1)^n))/6, n,k >=
 *            1. a(n) is the entry of the array A read by upward
 *            antidiagonals. The implementation uses binary search to find the
 *            diagonal and then evaluates the closed‑form formula using the
 *            provided pow2 and pow3 utilities
 * Range:     i8 n <= 6, i16 n <= 37, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Find the diagonal index d such that n <= d*(d+1)/2 */
static idx_t find_diagonal(idx_t n)
{
    idx_t low = 1;
    idx_t high = 1;
    while (high * (high + 1) / 2 < n) {
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n) {
            high = mid;
        } else {
            low = mid + 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine diagonal and coordinates (row, col) */
    idx_t diag = find_diagonal(n);                /* d */
    idx_t prev_cnt = (diag - 1) * diag / 2;      /* terms before this diagonal */
    idx_t offset = n - prev_cnt - 1;             /* 0‑based offset within the diagonal */
    idx_t row = diag - offset;                   /* n index in A */
    idx_t col = offset + 1;                      /* k index in A */

    /* Compute the formula a(row, col) */
    seq_t three_pow = pow3(row);                 /* 3^row */
    seq_t two_pow  = pow2(row + 1);              /* 2^(row+1) */
    seq_t base     = three_pow - two_pow;        /* 3^row - 2^(row+1) */

    /* (-1)^row */
    seq_t sign = ((row & 1) == 0) ? 1 : -1;

    /* (6*col - 3 + 2*(-1)^row) */
    seq_t term = (seq_t)(6 * col - 3 + 2 * sign);

    seq_t prod = base * term;                   /* (3^row - 2^(row+1)) * term */
    seq_t result = (1 + prod) / 6;              /* final value */

    return result;
}
