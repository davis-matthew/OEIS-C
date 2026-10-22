/*
 * A387751  a(n) = (1/8) * Sum_{k=0..n-1} binomial(8*n,8*k+1).
 * https://oeis.org/A387751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 120*a(n-1)+34800*a(n-2)+4096*a(n-3) for n>3 with initial
 *            values: a(0)=0, a(1)=1, a(2)=1432, a(3)=206704
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)1432;
    if (n == 3) return (seq_t)206704;

    seq_t a_nm3 = (seq_t)1;        /* a(1)   */
    seq_t a_nm2 = (seq_t)1432;     /* a(2)   */
    seq_t a_nm1 = (seq_t)206704;   /* a(3)   */
    seq_t next;

    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)120 * a_nm1
             + (seq_t)34800 * a_nm2
             + (seq_t)4096 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
