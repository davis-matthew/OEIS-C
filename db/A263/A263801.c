/*
 * A263801  Partial sums of odd double factorials (A001147) with alternating
 *          signs.
 * https://oeis.org/A263801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^k * (2*k-1)!!. Recurrence: a(0)=1,
 *            a(1)=0, a(n+2) = (2*n+3)*a(n) - (2*n+2)*a(n+1)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 17, i128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)0;

    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)0; /* a(1) */
    seq_t a_cur;

    for (idx_t i = 0; i < n - 1; ++i)
    {
        seq_t mul1 = (seq_t)(2 * i + 3); /* coefficient for a_nm2 */
        seq_t mul2 = (seq_t)(2 * i + 2); /* coefficient for a_nm1 */
        a_cur = mul1 * a_nm2 - mul2 * a_nm1;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_nm1;
}
