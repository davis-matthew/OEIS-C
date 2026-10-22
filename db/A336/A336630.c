/*
 * A336630  a(n) = 2*F(2*n+1) + 4*F(n+1)*F(n-1) for n > 0, with a(0) = 0 and
 *          F(n) = A000045(n).
 * https://oeis.org/A336630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A336630
 * Formula (recurrence): a(0)=0, a(1)=4, a(2)=18, a(3)=38,
 * a(n)=2*a(n-1)+2*a(n-2)-a(n-3)   for n>3
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)18;
    if (n == 3) return (seq_t)38;

    /* Iterate using the recurrence; keep only the last three terms */
    seq_t a_nm3 = (seq_t)0;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)4;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)18;  /* a(n-1) */
    seq_t a_n   = (seq_t)38;  /* a(n)   */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)2 * a_n + (seq_t)2 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
        a_n   = next;
    }
    return a_n;
}
