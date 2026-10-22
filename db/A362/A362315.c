/*
 * A362315  a(n) = n! * Sum_{k=0..floor(n/4)} (-n/4)^k /(k! * (n-4*k)!).
 * https://oeis.org/A362315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Sum_{k=0}^{floor(n/4)} (-n/4)^k/(k! * (n-4k)!)
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 19, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since for n<4 the sum is just the k=0 term, which is 1, we
       initialise the total with 1 and compute subsequent terms
       iteratively. */
    seq_t total = 1;          /* a(0) = 1, and also the contribution for k=0 */
    seq_t term  = 1;          /* term for current k; starts with k=0 */

    idx_t maxk = n / 4;       /* floor(n/4) */
    for (idx_t k = 0; k < maxk; ++k)
    {
        /* Compute the next term (k+1) from the current term (k).  The
           recurrence is:
               term_{k+1} = - term_k *
                            (n-4k)(n-4k-1)(n-4k-2)(n-4k-3) * n
                            / (4 * (k+1))   */
        seq_t mult_factor = (seq_t)(n - 4*k) *
                            (seq_t)(n - 4*k - 1) *
                            (seq_t)(n - 4*k - 2) *
                            (seq_t)(n - 4*k - 3);

        /* Multiply by the product of the next four descending integers. */
        term = term * mult_factor;

        /* Multiply by n (the numerator of (-n/4)^k+1). */
        term = term * (seq_t)n;

        /* Divide by 4*(k+1).  This division is exact for the
           sequence, so using integer division is safe. */
        seq_t denom = 4 * (seq_t)(k + 1);
        term = term / denom;

        /* Flip the sign for the factor (-1)^{k+1}. */
        term = -term;

        /* Add this term to the running total. */
        total += term;
    }

    return total;
}
