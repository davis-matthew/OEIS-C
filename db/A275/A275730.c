/*
 * A275730  Square array A(n,d): overwrite with zero the digit at position d
 *          from right (indicating radix d+2) in the factorial base
 *          representation of n, then convert back to decimal, read by
 *          descending antidiagonals as A(0,0), A(0,1), A(1,0), A(0,2),
 *          A(1,1), A(2,0), etc.
 * https://oeis.org/A275730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) is the sequence obtained by reading the square array A(n,d)
 *            by descending antidiagonals. A(n,d) is the number obtained from
 *            n by converting it into factorial base, setting the digit at
 *            position d (0‑based from the right) to zero, and converting back
 *            to decimal
 * Range:     u8 n <= 7259, u16 n <= 7259, u32 n <= 7259, u64 n <= 7259,
 *            u128 n <= 7259, big any n
 * Helpers:   factorial_zero_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/factorial_zero_digit.h"

/* Find diagonal t such that t*(t+1)/2 <= n < (t+1)*(t+2)/2 */
static inline idx_t diag_of(idx_t n)
{
    long double s = sqrtl((long double)(8.0L * n + 1.0L));
    idx_t t = (idx_t)((s - 1.0L) / 2.0L);

    /* adjust in case of rounding inaccuracies */
    while (((idx_t)(t + 1)) * ((idx_t)(t + 2)) / 2 <= n) ++t;
    while (t * ((idx_t)(t + 1)) / 2 > n) --t;
    return t;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* determine (row, col) by unfolding n into antidiagonals */
    idx_t t   = diag_of(n);
    idx_t off = n - t * (t + 1) / 2;   /* offset within diagonal t */
    idx_t row = off;                   /* n argument of A(n,d) */
    idx_t col = t - off;               /* d argument of A(n,d) */
    return factorial_zero_digit((seq_t)row, col);
}
