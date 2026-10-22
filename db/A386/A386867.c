/*
 * A386867  a(n) = Sum_{k=0..n} 2^(n-k) * binomial(4*n+2,k) *
 *          binomial(4*n-k-1,n-k).
 * https://oeis.org/A386867
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 2^(n-k) * binomial(4*n+2,k) *
 *            binomial(4*n-k-1,n-k)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses the equivalent summation
   a(n) = Sum_{k=0..n} 3^k * binomial(3*n + k - 1, k) * binomial(n - k + 2, 2)    */
/* which allows computation in O(n) time with only integer arithmetic. */
seq_t a(idx_t n)
{
    seq_t sum = 0;          /* result accumulator            */
    seq_t pow3 = 1;         /* current value of 3^k           */
    seq_t A = 1;            /* A_k = binomial(3*n + k - 1, k) */

/* iterate k from 0 up to n inclusive */
    for (idx_t k = 0; k <= n; ++k)
    {
        /* B_k = binomial(n - k + 2, 2) = (t*(t-1))/2,  t = n-k+2 */
        idx_t t = n - k + 2;
        seq_t B = (seq_t)(t * (t - 1)) / 2;

        /* term = pow3 * A * B, all in seq_t arithmetic */
        seq_t term = pow3;
        term = term * A;    /* pow3 * A   */
        term = term * B;    /* pow3 * A * B */

        sum = sum + term;   /* accumulate  */

        /* prepare values for next iteration, unless k==n */
        if (k == n) break;

        /* A_{k+1} = A_k * (3*n + k) / (k+1) */
        idx_t top = 3 * n + k;       /* 3*n + k   */
        A = (A * (seq_t)top) / (k + 1);

        /* 3^(k+1) = 3^k * 3 */
        pow3 = pow3 * 3;
    }
    return sum;
}
