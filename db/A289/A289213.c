/*
 * A289213  a(n) = n! * Laguerre(n,-7).
 * https://oeis.org/A289213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Laguerre(n,-7) Recurrence: a(0)=1, a(1)=8, a(n+1) =
 *            (2n+8)*a(n) - n^2*a(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)1;
    if (n == 1u)
        return (seq_t)8;

    seq_t a_nm2 = (seq_t)1;   /* a(0) */
    seq_t a_nm1 = (seq_t)8;   /* a(1) */
    seq_t a_n   = 0;
    for (idx_t i = 1; i < n; ++i)   /* i runs 1..n-1, compute a(i+1) */
    {
        /* term1 = (2*i + 8) * a_nm1 */
        seq_t term1 = (seq_t)(2 * i + 8);
        term1 *= a_nm1;

        /* term2 = i*i * a_nm2 */
        seq_t temp = (seq_t)i;
        temp *= temp;
        temp *= a_nm2;

        a_n = term1 - temp;

        /* shift for next iteration */
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
