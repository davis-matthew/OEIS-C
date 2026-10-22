/*
 * A356198  Number of edge covers in the n-book graph.
 * https://oeis.org/A356198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*5^n - 2^(n+1) - 1 Recurrence used: a(1)=5, a(2)=41,
 *            a(3)=233; a(n)=8*a(n-1)-17*a(n-2)+10*a(n-3) for n≥4
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if (n == 1)
        return 5;
    if (n == 2)
        return 41;
    if (n == 3)
        return 233;

    seq_t a_nm3 = 5;    /* a(n-3) */
    seq_t a_nm2 = 41;   /* a(n-2) */
    seq_t a_nm1 = 233;  /* a(n-1) */
    seq_t a_n = 0;

    for (idx_t i = 4; i <= n; ++i) {
        a_n = 8 * a_nm1 - 17 * a_nm2 + 10 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
