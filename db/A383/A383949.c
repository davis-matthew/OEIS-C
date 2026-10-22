/*
 * A383949  Expansion of 1/sqrt((1-x)^3 * (1-5*x)).
 * https://oeis.org/A383949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=4; for n>1, a(n) = 6*a(n-1) - 5*a(n-2) -
 *            (2*a(n-1))/n (exact integer division)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    seq_t a_nm2 = (seq_t)1; /* a(0) */
    seq_t a_nm1 = (seq_t)4; /* a(1) */
    seq_t a_n   = a_nm1;    /* temporary holder */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t term1 = a_nm1 * (seq_t)6;        /* 6 * a(n-1) */
        seq_t term2 = a_nm2 * (seq_t)5;        /* 5 * a(n-2) */
        seq_t term3 = a_nm1 * (seq_t)2;        /* 2 * a(n-1) */
        seq_t div   = term3 / (seq_t)i;        /* (2*a(n-1))/n */

        a_n = term1 - term2 - div;

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
