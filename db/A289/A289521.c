/*
 * A289521  Number of vertices in a planar Apollonian graph at iteration n.
 * https://oeis.org/A289521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5 + 3^n)/2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(1)=4, a(2)=7, a(n)=4*a(n-1)-3*a(n-2) for n>2 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)7;

    seq_t a_nm2 = (seq_t)4;      /* a(n-2) */
    seq_t a_nm1 = (seq_t)7;      /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        a_n = 4 * a_nm1 - 3 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
