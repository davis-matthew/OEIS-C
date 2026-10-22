/*
 * A341543  a(n) = sqrt( Product_{j=1..n} Product_{k=1..2}
 *          (4*sin((2*j-1)*Pi/(2*n))^2 + 4*sin((2*k-1)*Pi/2)^2) ).
 * https://oeis.org/A341543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - a(n-2) - 8 Parameters: a(1) = 8, a(2) = 36
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)8;
    if (n == 2)
        return (seq_t)36;

    seq_t prev2 = (seq_t)8;   /* a(n-2) */
    seq_t prev1 = (seq_t)36;  /* a(n-1) */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)6 * prev1;  /* 6*a(n-1) */
        cur -= prev2;            /* - a(n-2) */
        cur -= (seq_t)8;         /* - 8 */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
