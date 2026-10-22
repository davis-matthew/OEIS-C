/*
 * A398309  Number of indecomposable closed walks of length n on the Petersen
 *          graph based at a vertex.
 * https://oeis.org/A398309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(0)=1; a(1)=0; a(2)=3; a(3)=0; a(n)=2*a(n-1)+2*a(n-2) for n>3
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 46, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)0;

    seq_t a_nm2 = (seq_t)3;   /* a(2) */
    seq_t a_nm1 = (seq_t)0;   /* a(3) */
    seq_t cur = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)2 * a_nm1 + (seq_t)2 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }

    return cur;
}
