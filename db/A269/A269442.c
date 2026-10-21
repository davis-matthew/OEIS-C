/*
 * A269442  a(n) = n*(n^8 + 1)*(n^4 + 1)*(n^2 + 1)*(n + 1) + 1.
 * https://oeis.org/A269442
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^8 + 1)*(n^4 + 1)*(n^2 + 1)*(n + 1) + 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 15, u128 n <= 255,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;          /* n^2 */
    seq_t n4 = n2 * n2;        /* n^4 */
    seq_t n8 = n4 * n4;        /* n^8 */
    seq_t res = N * (n8 + 1);  /* n*(n^8+1) */
    res *= (n4 + 1);           /* * (n^4+1) */
    res *= (n2 + 1);           /* * (n^2+1) */
    res *= (N + 1);            /* * (n+1) */
    res += 1;                  /* +1 */
    return res;
}
