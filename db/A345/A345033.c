/*
 * A345033  Square array T(n,k), n >= 1, k >= 0, read by antidiagonals
 *          downwards, where T(n,k) = Sum_{j=1..n} (-k)^(floor(n/j) - 1).
 * https://oeis.org/A345033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(row,col) where T(n,k) = Σ_{j=1}^{n} (-k)^{⌊n/j⌋–1}, and
 *            (row,col) are the coordinates obtained from the n‑th term of the
 *            square array read by antidiagonals downwards (offset 1)
 * Range:     i8 n <= 40, i16 n <= 78, i32 n <= 78, i64 n <= 78, i128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the diagonal d with (d+1)(d+2)/2 ≥ n */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 < n) d++;

    /* Position of n inside this diagonal */
    idx_t prior = d * (d + 1) / 2;       /* elements before diagonal d    */
    idx_t t     = n - prior - 1;         /* 0‑based offset in diagonal    */
    idx_t row   = t + 1;                 /* row index (1‑based)           */
    idx_t col   = d - t;                 /* column index (0‑based)        */

    /* Pre‑compute powers of (-col) up to exponent row-1 */
    static seq_t *pow_arr = NULL;
    static size_t pow_size = 0;
    if (pow_size < (size_t)row) {
        pow_arr = (seq_t *)realloc(pow_arr, row * sizeof(seq_t));
        pow_size = (size_t)row;
    }
    pow_arr[0] = 1;
    seq_t base = -(seq_t)col;
    for (idx_t e = 1; e < row; ++e) {
        pow_arr[e] = pow_arr[e - 1] * base;
    }

    /* Sum the required terms */
    seq_t sum = 0;
    for (idx_t j = 1; j <= row; ++j) {
        idx_t exp = row / j - 1;          /* ⌊row/j⌋–1  ≥ 0 */
        sum += pow_arr[(size_t)exp];
    }
    return sum;
}
