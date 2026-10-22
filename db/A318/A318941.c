/*
 * A318941  Number of Dyck paths with n nodes and altitude 2.
 * https://oeis.org/A318941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 7*a(n-2) + 2*a(n-3) for n > 5 Initial values:
 *            a(0)=0, a(1)=0, a(2)=1, a(3)=4, a(4)=12, a(5)=35
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return base cases directly. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)4;
    if (n == 4) return (seq_t)12;
    if (n == 5) return (seq_t)35;

    /* Work iteratively using the recurrence. */
    seq_t a_nm3 = (seq_t)4;   /* a(3) */
    seq_t a_nm2 = (seq_t)12;  /* a(4) */
    seq_t a_nm1 = (seq_t)35;  /* a(5) */

    for (idx_t i = 6; i <= n; ++i)
    {
        seq_t curr = 5 * a_nm1 - 7 * a_nm2 + 2 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }
    return a_nm1;
}
