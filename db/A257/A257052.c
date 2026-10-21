/*
 * A257052  a(n) = cpg(3, n) + cpg(4, n) + ... + cpg(n, n) where cpg(m, n) is
 *          the n-th m-th-order centered polygonal number.
 * https://oeis.org/A257052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4-7*n^2+10*n-8)/4 for n>1 a(0) = 0, a(1) = 0, a(2) = 0
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 284, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return 0;
    seq_t x = (seq_t)n;
    seq_t term = x * x;               /* x^2 */
    term *= term;                    /* x^4 */

    seq_t x2 = x * x;                 /* x^2 again */
    term -= 7 * x2;                   /* x^4 - 7*x^2 */
    term += 10 * x;                   /* + 10*x */
    term -= 8;                        /* - 8 */

    return term / 4;                  /* divide by 4 */
}
