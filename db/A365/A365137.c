/*
 * A365137  a(n) is the number of n-digit numbers that contain '22' in their
 *          decimal representation.
 * https://oeis.org/A365137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 19*a(n-1) - 81*a(n-2) - 90*a(n-3) with a(0) = 0, a(1) =
 *            0, a(2) = 1, a(3) = 18
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four values explicitly */
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)0;
    if (n == 2u) return (seq_t)1;
    if (n == 3u) return (seq_t)18;

    /* use only three previous terms */
    seq_t a_nm3 = (seq_t)0;   /* a(n-3) for the current i==4 */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) for the current i==4 */
    seq_t a_nm1 = (seq_t)18;  /* a(n-1) for the current i==4 */
    seq_t a_cur;

    for (idx_t i = 4u; i <= n; ++i)
    {
        a_cur = 19 * a_nm1 - 81 * a_nm2 - 90 * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }
    return a_nm1;
}
