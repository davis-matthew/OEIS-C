/*
 * A385342  Square array A(n,k) read by descending antidiagonals, where each
 *          row n starts with an ordered list of positive integers then we
 *          swap A(n,k) with the number in the position A(n, A(n,k*n)).
 * https://oeis.org/A385342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A385342: a(n) = value in the square array read by descending anti‑diagonals.
   The array entries are defined by
        A(n,1) = n,
        for n > 1 let t be the exponent of n dividing k,
        if t is odd then A(n,k) = k / n
        else          A(n,k) = n * k.
   (For n = 1, A(1,k) = k.) */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, but safety */

    /* Find the diagonal d that contains the n‑th term
       (sum_{i=1}^{d} i >= n > sum_{i=1}^{d-1} i).                */
    long double nd = (long double)n;
    long double disc = sqrtl(8.0L * nd + 1.0L);
    idx_t d = (idx_t)((disc - 1.0L) / 2.0L);   /* floor, may be one too small */
    /* Adjust d to guarantee d(d+1)/2 >= n */
    while ((unsigned __int128)d * (d + 1) / 2 < n)
        d++;
    while (d > 0 && ((unsigned __int128)(d - 1) * d) / 2 >= n)
        d--;

    /* Compute (row, col) indices within the array */
    idx_t prev = ((unsigned __int128)(d - 1) * d) / 2;
    idx_t offset = n - prev;          /* 1‑based position inside diagonal  */
    idx_t i = d - offset + 1;         /* row index   */
    idx_t j = offset;                 /* column index */

    /* Evaluate A(i,j) according to the rule above */
    if (i == 1)
        return (seq_t)j;              /* natural numbers row 1 */

    /* Count how many times i divides j */
    idx_t t = 0;
    idx_t tmp = j;
    while (tmp % i == 0) {
        tmp /= i;
        t++;
    }

    if (t & 1)   /* odd exponent -> divide */
        return ((seq_t)j) / ((seq_t)i);
    else         /* even exponent -> multiply */
        return ((seq_t)i) * ((seq_t)j);
}
