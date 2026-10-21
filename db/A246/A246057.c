/*
 * A246057  a(n) = (5*10^n - 2)/3.
 * https://oeis.org/A246057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*a(n-1) - 10*a(n-2), a(0)=1, a(1)=16
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)16;
    seq_t a_nm2 = (seq_t)1;
    seq_t a_nm1 = (seq_t)16;
    seq_t a_n = (seq_t)0;
    for (idx_t i = 2; i <= n; ++i) {
        a_n = (seq_t)11 * a_nm1 - (seq_t)10 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
