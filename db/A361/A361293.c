/*
 * A361293  a(n) = 20 * a(n-1) - 90 * a(n-2) for n>1, with a(0)=0, a(1)=1.
 * https://oeis.org/A361293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20 * a(n-1) - 90 * a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) { return (seq_t)0; }
    if (n == 1) { return (seq_t)1; }

    seq_t a_nm2 = (seq_t)0;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)1;   /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = ((seq_t)20 * a_nm1) - ((seq_t)90 * a_nm2);
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return a_nm1;
}
