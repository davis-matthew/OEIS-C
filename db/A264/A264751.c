/*
 * A264751  Triangle read by rows: T(n,k) is the number of sequences of k <= n
 *          throws of an n-sided die (with faces numbered 1, 2, ..., n) in
 *          which the sum of the throws first reaches or exceeds n on the k-th
 *          throw.
 * https://oeis.org/A264751
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the nth element of the triangle read by rows of A264751.
 *            * The triangle is defined by T(n,k) = number of sequences of k
 *            (≤ n) throws of * an n‑sided die such that the running total
 *            first reaches or exceeds n on * the k‑th throw. The elements are
 *            read row‑major: a(1)=T(1,1), a(2)=T(2,1), *...
 * Range:     u8 n <= 7, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"

static idx_t find_row(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 >= n. */
    seq_t nn = (seq_t)n;
    seq_t val = nn * (seq_t)8 + 1;            /* 8*n + 1    */
    seq_t sq  = floor_sqrt(val);              /* floor(sqrt) */
    idx_t r = (idx_t)((sq - 1) / 2);           /* initial estimate */

    /* Adjust in case of rounding errors. */
    while (((seq_t)r * (r + 1)) / 2 < nn) ++r;
    while (r > 0 && ((seq_t)(r - 1) * r) / 2 >= nn) --r;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset is 1; behaviour for n<1 is irrelevant */

    idx_t r = find_row(n);                 /* row index */
    seq_t prev_sum = ((seq_t)(r - 1) * r) / 2; /* sum of all rows before r */
    idx_t k = (idx_t)((seq_t)n - prev_sum);    /* column index in row r */

    if (k <= 1) return (seq_t)1;          /* T(r,1) = 1 */

    /* Compute T(r,k) = Σ_{i=k}^{r} i * C(i-2, k-2). */
    seq_t result = 0;
    idx_t i;
    for (i = k; i <= r; ++i) {
        seq_t bin = binomial(i - 2, k - 2);
        result += (seq_t)i * bin;
    }
    return result;
}
