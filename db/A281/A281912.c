/*
 * A281912  Number of sequences of balls colored with at most n colors such
 *          that exactly one ball is of a color seen earlier in the sequence.
 * https://oeis.org/A281912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ( ((n+2)/(n-1))*a(n-1) - a(n-2) ), with initial
 *            values a(1)=1, a(2)=8
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(idx_t n) for OEIS A281912.
   Uses only integer arithmetic, no floating point.
   Computes iteratively to keep memory usage O(1). */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)8;

    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)8;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Compute ((i+2)/(i-1))*a_nm1 safely:
           a_nm1 is divisible by (i-1). */
        seq_t div   = a_nm1 / (i - 1);          /* a_nm1 / (i-1)   */
        seq_t temp  = (seq_t)(i + 2) * div;    /* (i+2) * div     */
        seq_t sub   = temp - a_nm2;             /* temp - a_nm2    */
        seq_t a_i   = (seq_t)i * sub;          /* i * (temp-a_nm2) */

        a_nm2 = a_nm1;
        a_nm1 = a_i;
    }

    return a_nm1;
}
