/*
 * A279007  Values tilde(B_s(2)) of q-analogs of Fibonacci numbers.
 * https://oeis.org/A279007
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) + 2*a(n-3); a(0)=0, a(1)=2, a(2)=1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)1;

    seq_t a_nm3 = (seq_t)0; /* a(n-3) */
    seq_t a_nm2 = (seq_t)2; /* a(n-2) */
    seq_t a_nm1 = (seq_t)1; /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)5 * a_nm2 + (seq_t)2 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = next;
    }
    return a_nm1;
}
