/*
 * A292131  Square array A(n,k), n>=0, k>=0, read by antidiagonals, where
 *          column k is the expansion of Product_{j>=1} (1 - k*x^j).
 * https://oeis.org/A292131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   For each n ≥ 0, let d = ⌊(√(8n+1) - 1)/2⌋ (the diagonal number)
 *            i = n - d(d+1)/2 (row index on that diagonal) j = d - i (column
 *            index on that diagonal) Then a(n) is the coefficient of x^i in
 *            the infinite product ∏_{m≥1} (1 - j x^m). The product can be
 *            truncated to the first i factors because high...
 * Range:     i8 n <= 83, i16 n <= 312, i32 n <= 1357, i64 n <= 6859,
 *            i128 n <= 9869, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>   /* sqrt */

static idx_t floor_sqrt(idx_t x)
{
    return (idx_t) sqrt((long double) x);
}

seq_t a(idx_t n)
{
    /* diagonal index */
    idx_t d = (floor_sqrt((idx_t) (8 * (long long) n + 1)) - 1) / 2;
    /* offset on that diagonal */
    idx_t T_d = d * (d + 1) / 2;
    idx_t r = n - T_d;
    /* row and column indices */
    idx_t i = r;          /* coefficient index */
    idx_t j = d - r;      /* column index */

    /* maximum i that can appear for n ≤ 10000 is < 200 */
    static const int MAX_I = 200;
    seq_t cur[MAX_I + 1];
    for (idx_t k = 0; k <= i; ++k) cur[k] = 0;
    cur[0] = 1;

    if (i == 0)
        return cur[0];               /* coefficient of x^0 is always 1 */

    seq_t factor = -(seq_t) j;        /* (-j) factor for each part */

    for (idx_t m = 1; m <= i; ++m)          /* for parts 1 … i */
    {
        for (idx_t sum = i; sum >= m; --sum) /* update from high to low */
        {
            cur[sum] += cur[sum - m] * factor;
        }
    }
    return cur[i];
}
