/*
 * A359335  Square root of determinant of skew-symmetric 2n X 2n matrix with
 *          entries i XOR j for i < j, i=1..2n, j=1..2n.
 * https://oeis.org/A359335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=3. For n>1: a(n)=a(n-1)*2*(4*e(n)-1)/(2*e(n)-1),
 *            where e(n) is the largest power of 2 dividing n (e(n)=n&-n)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t a = (seq_t)3;                 /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        idx_t e = i & (idx_t)(-i);     /* largest power of 2 dividing i */
        seq_t num = (seq_t)8 * e - 2;  /* 2*(4*e-1) = 8*e-2  */
        seq_t den = (seq_t)2 * e - 1;  /* 2*e-1           */
        a = a * num / den;             /* exact division   */
    }
    return a;
}
