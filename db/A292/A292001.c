/*
 * A292001  Number of (undirected) paths in the n-helm graph.
 * https://oeis.org/A292001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(4*n^3 -12*n^2 +38*n -21)/3
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* distance from offset; not used */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;
    seq_t n3 = n2 * sn;
    seq_t val = (seq_t)4 * n3;      /* 4*n^3 */
    val += (seq_t)38 * sn;          /* +38*n */
    val -= (seq_t)12 * n2;          /* -12*n^2 */
    val -= (seq_t)21;               /* -21 */
    val = sn * val;                 /* *n */
    val /= (seq_t)3;                /* /3 */
    return val;
}
