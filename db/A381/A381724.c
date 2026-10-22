/*
 * A381724  a(n) = pos(M(n)), where M(n) is the n X n matrix with every term =
 *          4, and pos(M(n)) is the positive part of the determinant of M(n);
 *          see A380661.
 * https://oeis.org/A381724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=4, a(2)=16, a(n)=4*n*a(n-1) for n>=3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* outside the offset – not required */
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;

    seq_t res = (seq_t)16;
    for (idx_t i = 3; i <= n; ++i) {
        res *= (seq_t)(4 * i);
    }
    return res;
}
