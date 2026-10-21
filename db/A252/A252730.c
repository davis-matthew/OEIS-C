/*
 * A252730  a(n) = P_n(n) with P_0(z) = z+1 and P_n(z) = z +
 *          P_{n-1}(z)*(P_{n-1}(z)-z) for n>1.
 * https://oeis.org/A252730
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=P_n(n) where P_0(z)=z+1 and
 *            P_n(z)=z+P_{n-1}(z)*(P_{n-1}(z)-z) for n>0
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Start with P_0(n) = n+1. */
    seq_t p = (seq_t)n + 1;

    /* Iteratively apply P_k(z) at z = n for k=1..n. */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t diff = p - (seq_t)n;        /* p - n  */
        p = (seq_t)n + p * diff;          /* n + p*(p-n) */
    }
    return p;
}
