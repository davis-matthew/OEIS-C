/*
 * A304934  a(0) = 0, a(1) = 1 and a(n) = 2*a(n-1)/(n-1) + 64*a(n-2) for n >
 *          1.
 * https://oeis.org/A304934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0, a(1) = 1; a(n) = 2*a(n-1)/(n-1) + 64*a(n-2) for n > 1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)0; /* a(0) */
    seq_t a_nm1 = (seq_t)1; /* a(1) */
    seq_t a_n   = a_nm1;    /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        a_n = ((seq_t)2 * a_nm1) / (seq_t)(i - 1);
        a_n += (seq_t)64 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
