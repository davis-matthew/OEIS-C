/*
 * A370671  a(n) = Sum_{i=2..n-1} i*n^(i-2).
 * https://oeis.org/A370671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=2..n-1} i * n^(i-2) (a(0)=0, a(1)=0, a(2)=0)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n ≤ 2 the sum is empty. */
    if (n <= 2)
        return 0;

    seq_t sum   = 0;                /* running total */
    seq_t pow   = 1;                /* n^(i-2) for current i */
    idx_t i;

    for (i = 2; i <= n - 1UL; ++i)  /* iterate i from 2 to n-1 inclusive */
    {
        /* term = i * n^(i-2) */
        sum += (seq_t)i * pow;
        /* prepare for next iteration: pow = n^(i-1) */
        pow *= (seq_t)n;
    }

    return sum;
}
