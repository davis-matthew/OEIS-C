/*
 * A292000  Number of (undirected) paths in the n-gear graph.
 * https://oeis.org/A292000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A292000: Number of (undirected) paths in the n‑gear graph.
   Formula: a(n) = n*(-21 + 58*n - 15*n^2 + 8*n^3)/6
   (valid for n ≥ 1; a(0) is undefined, return 0). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;          /* n */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */

    seq_t inner = (seq_t)8 * n3;   /* 8*n^3 */
    inner -= (seq_t)15 * n2;       /* -15*n^2 */
    inner += (seq_t)58 * nn;       /* +58*n */
    inner -= (seq_t)21;            /* -21 */

    seq_t res = nn * inner / 6;    /* n * inner / 6 */
    return res;
}
