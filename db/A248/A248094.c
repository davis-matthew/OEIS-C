/*
 * A248094  The hyper-Wiener index of the hexagonal triangle T_n, defined in
 *          the He et al. reference.
 * https://oeis.org/A248094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(66 + 407*n + 670*n^2 + 425*n^3 + 104*n^4 + 8*n^5)/40
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;            /* convert index to seq_t */
    seq_t n2 = N * N;              /* n^2 */
    seq_t n3 = n2 * N;             /* n^3 */
    seq_t n4 = n3 * N;             /* n^4 */
    seq_t n5 = n4 * N;             /* n^5 */

    seq_t poly = (seq_t)66;
    poly += (seq_t)407 * N;        /* + 407*n */
    poly += (seq_t)670 * n2;       /* + 670*n^2 */
    poly += (seq_t)425 * n3;       /* + 425*n^3 */
    poly += (seq_t)104 * n4;       /* + 104*n^4 */
    poly += (seq_t)8   * n5;       /* + 8*n^5 */

    seq_t num = N * poly;          /* n * poly */
    return num / (seq_t)40;        /* divide by 40 */
}
