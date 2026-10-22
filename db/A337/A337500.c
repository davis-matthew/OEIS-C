/*
 * A337500  a(n) is the number of ballot sequences of length n tied or won by
 *          at most 3 votes.
 * https://oeis.org/A337500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) satisfies -(n+3)*(n-4)*a(n) + 2*(n^2-2*n-11)*a(n-1) +
 *            4*(n-1)^2*a(n-2) - 8*(n-1)*(n-2)*a(n-3) = 0 with initial values
 *            a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=14
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms directly. */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;
    if (n == (idx_t)2) return (seq_t)4;
    if (n == (idx_t)3) return (seq_t)8;
    if (n == (idx_t)4) return (seq_t)14;

    /* a(n-3), a(n-2), a(n-1) for current n. */
    seq_t a_nm3 = (seq_t)4;   /* a(2) */
    seq_t a_nm2 = (seq_t)8;   /* a(3) */
    seq_t a_nm1 = (seq_t)14;  /* a(4) */

    for (idx_t i = (idx_t)5; i <= n; ++i)
    {
        /* Compute the numerator of the recurrence:
           2*(i^2-2*i-11)*a(i-1) + 4*(i-1)^2*a(i-2) - 8*(i-1)*(i-2)*a(i-3)
        */
        seq_t ii = (seq_t)i * (seq_t)i;
        seq_t term1_coeff = (seq_t)2 * (ii - (seq_t)2 * (seq_t)i - (seq_t)11);
        seq_t term1 = term1_coeff * a_nm1;

        seq_t sm1 = (seq_t)(i - 1);
        seq_t term2_coeff = (seq_t)4 * sm1 * sm1;
        seq_t term2 = term2_coeff * a_nm2;

        seq_t term3_coeff = (seq_t)8 * sm1 * (seq_t)(i - 2);
        seq_t term3 = term3_coeff * a_nm3;

        seq_t numerator = term1 + term2 - term3;

        seq_t denom = (seq_t)(i + 3) * (seq_t)(i - 4);

        seq_t an = numerator / denom;

        /* Shift for next step. */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = an;
    }

    return a_nm1; /* a(n) */
}
