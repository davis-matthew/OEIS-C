/*
 * A331528  a(n) = n^2 * (n+1)^2 * (n^2+n+1) / 12.
 * https://oeis.org/A331528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n+1)^2 * (n^2+n+1) / 12
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;           /* n */
    seq_t n1 = nn + 1;             /* n+1 */
    seq_t t  = nn * n1;            /* n*(n+1) */
    seq_t a1 = t / 2;              /* (n*(n+1))/2 */
    seq_t a2 = a1 * a1;            /* ((n*(n+1))/2)^2 */
    seq_t w  = nn * nn + nn + 1;   /* n^2 + n + 1 */
    if (w % 3 == 0)
        w /= 3;                    /* divide the odd factor */
    else
        a2 /= 3;                   /* ensure integer division */
    return a2 * w;                 /* final result */
}
