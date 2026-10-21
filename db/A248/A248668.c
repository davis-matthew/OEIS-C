/*
 * A248668  Sum of the numbers in the n-th row of the array at A248664.
 * https://oeis.org/A248668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n-1} k!*binomial(2*n-1,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The computation uses an iterative evaluation of the sum.
       fact   = k!      (initially 1 for k = 0)
       binom  = binomial(2*n-1, k) (initially 1 for k = 0)
       For each k, add fact * binom to the running sum.
       Then update fact and binom for k+1, unless k = n-1. */
    seq_t sum = 0;
    seq_t binom = 1;     /* C(2*n-1,0) */
    seq_t fact = 1;      /* 0! */

    for (idx_t k = 0; k < n; ++k) {
        sum += fact * binom;

        /* Update for next k, if any */
        if (k + 1 < n) {
            fact *= (seq_t)(k + 1);

            seq_t numerator = binom * (seq_t)(2 * n - 1 - k);
            binom = numerator / (seq_t)(k + 1);
        }
    }
    return sum;
}
