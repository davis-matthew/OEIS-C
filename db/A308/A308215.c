/*
 * A308215  a(n) is the multiplicative inverse of A001844(n+1) modulo
 *          A001844(n); where A001844 is the sequence of centered square
 *          numbers.
 * https://oeis.org/A308215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0 a(n) = (3 + (-1)^n + 2*(2+(-1)^n)*n + 2*(3+(-1)^n)*n^2)
 *            / 4 for n>0 Equivalently: if n even: a(n) = (4 + 6n + 8n^2) / 4
 *            if n odd : a(n) = (2 + 2n + 4n^2) / 4 This satisfies: a(n) *
 *            (2*n*(n+1)+1) == 1 (mod) (2*n*(n-1)+1)
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;   /* n^2 */

    if ((n & 1) == 0)   /* n even */
    {
        /* a(n) = (4 + 6n + 8n^2) / 4 */
        return (4 + 6 * sn + 8 * n2) / 4;
    }
    else                /* n odd */
    {
        /* a(n) = (2 + 2n + 4n^2) / 4 */
        return (2 + 2 * sn + 4 * n2) / 4;
    }
}
