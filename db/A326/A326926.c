/*
 * A326926  Triangular array read by rows: row n shows the coefficients of
 *          this polynomial of degree n: (1/n!)*(numerator of n-th derivative
 *          of 1/(1-x+x^2)).
 * https://oeis.org/A326926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 23, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:
 * Let D(x) = 1 - x + x^2, D'(x) = -1 + 2x.
 * Define P_0(x) = 1.
 * For n ≥ 0,
 *     P_{n+1}(x) = (P'_n(x) * D(x) - (n+1) * P_n(x) * D'(x)) / (n+1).
 * The n‑th a(n) is the coefficient of x^k in P_r(x) where
 *   r = ⌊(√(8n+1)-1)/2⌋   (row index) and
 *   k = n - r(r+1)/2        (column index within that row).
 * The coefficients are computed exactly using integer arithmetic.
 */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are supplied by the build system. */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;          /* column index within the row, 0‑based */

    if (r == 0)                    /* row 0 has only the constant 1 */
        return 1;

    /* Allocate polynomial coefficient arrays.
       cur holds P_{m} and next will hold P_{m+1}. */
    size_t max_deg = (size_t)r;
    seq_t *cur  = (seq_t*)malloc((max_deg + 1) * sizeof(seq_t));
    seq_t *next = (seq_t*)malloc((max_deg + 2) * sizeof(seq_t)); /* size m+1 */

    cur[0] = 1;                    /* P_0(x) = 1 */
    size_t cur_deg = 0;             /* current polynomial degree */

    for (idx_t m = 1; m <= r; ++m) {
        size_t next_deg = (size_t)m;
        /* Zero out next array up to its degree. */
        for (size_t i = 0; i <= next_deg; ++i)
            next[i] = 0;

        idx_t factor = m; /* (n+1) in the recurrence */

        /* part1: derivative of P_{m-1} multiplied by D(x). */
        for (size_t j = 1; j <= cur_deg; ++j) {
            seq_t deriv = (seq_t)j * cur[j];      /* derivative coefficient at degree j-1 */
            size_t base = j - 1;                  /* degree of derivative term */
            next[base]     += deriv;              /* * 1   (x^0 term of D) */
            next[base + 1] -= deriv;              /* * -1  (x^1 term of D) */
            next[base + 2] += deriv;              /* * 1   (x^2 term of D) */
        }

        /* part2: subtract factor * P_{m-1} * D'(x) = factor * P_{m-1} * (-1 + 2x). */
        for (size_t j = 0; j <= cur_deg; ++j) {
            seq_t coeff = cur[j];
            /* coefficient at degree j: factor * coeff * (-1)   => subtracting gives +factor*coeff */
            next[j]     += factor * coeff;           /* subtracting -factor*coeff = adding factor*coeff */
            /* coefficient at degree j+1: factor * coeff * 2   => subtracting gives -factor*coeff*2 */
            next[j + 1] -= factor * coeff * 2;
        }

        /* Divide by factor to obtain integer coefficients. */
        for (size_t i = 0; i <= next_deg; ++i)
            next[i] /= (seq_t)factor;

        /* Prepare for next iteration.  Swap pointers. */
        seq_t *tmp  = cur;
        cur = next;
        next = tmp;
        cur_deg = next_deg;
    }

    seq_t result = cur[k];
    free(cur);
    free(next);
    return result;
}
