/*
 * A249131  p(1,n), where the polynomial p(n,x) is defined in Comments; sum of
 *          the numbers in row n of the triangular array at A249130.
 * https://oeis.org/A249131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3; for n>=2, a(n)=a(n-1)+((n%2==0)?n:n+1)*a(n-2)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    seq_t a0 = (seq_t)1, a1 = (seq_t)3, an;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t coef = (seq_t)((i & 1) ? (i + 1) : i); /* (n%2==0)?n:n+1 */
        an = a1 + coef * a0;
        a0 = a1;
        a1 = an;
    }
    return a1;
}
