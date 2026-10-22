/*
 * A303108  a(n) = (2*n-1)*a(n-1) - (n-2)!, with a(1) = 2, n > 1.
 * https://oeis.org/A303108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)*a(n-1) - (n-2)!, a(1)=2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 2;

    seq_t prev = 2;           /* a(1)   */
    seq_t fact = 1;           /* (n-2)! for current i, starts with 0! */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = prev * (seq_t)(2 * i - 1) - fact;
        prev = term;
        fact *= (i - 1);     /* prepare (i-1)! for next iteration */
    }
    return prev;
}
