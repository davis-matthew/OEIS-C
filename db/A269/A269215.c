/*
 * A269215  Number of 2 X n 0..3 arrays with some element plus some
 *          horizontally, diagonally or antidiagonally adjacent neighbor
 *          totalling three exactly once.
 * https://oeis.org/A269215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10*a(n-1) - 13*a(n-2) - 60*a(n-3) - 36*a(n-4), with
 *            a(1)=0, a(2)=96, a(3)=768, a(4)=6528
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 22, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)0;
    if (n == (idx_t)2)
        return (seq_t)96;
    if (n == (idx_t)3)
        return (seq_t)768;
    if (n == (idx_t)4)
        return (seq_t)6528;

    /* Use a sliding window of last four values. */
    seq_t a_nm4 = (seq_t)0;          /* a(n-4)   */
    seq_t a_nm3 = (seq_t)96;         /* a(n-3)   */
    seq_t a_nm2 = (seq_t)768;        /* a(n-2)   */
    seq_t a_nm1 = (seq_t)6528;       /* a(n-1)   */
    seq_t a_n   = (seq_t)0;          /* a(n)     */

    for (idx_t i = (idx_t)5; i <= n; ++i) {
        a_n = a_nm1 * (seq_t)10
            - a_nm2 * (seq_t)13
            - a_nm3 * (seq_t)60
            - a_nm4 * (seq_t)36;

        /* Shift window for next iteration */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}
