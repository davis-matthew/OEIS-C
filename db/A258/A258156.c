/*
 * A258156  Curl kernel sequence: a(n) is the number of independent constants
 *          on which an n-th-order tensor h can be shown to depend if the
 *          equation [curl (h nabla^{n-2}curl v)=0] is satisfied nontrivially
 *          for all vectors v.
 * https://oeis.org/A258156
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 0 (n = 2); otherwise a(n) = 3^n - (7*n^2 - 17*n + 30)/2
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 2 */
    if (n == 2) return 0;

    /* Compute 3^n */
    seq_t pow3 = 1;
    for (idx_t i = 0; i < n; ++i)
        pow3 *= (seq_t)3;

    /* Compute the quadratic term q(n) = (7*n^2 - 17*n + 30)/2 */
    seq_t nn = (seq_t)n;
    seq_t q = (((seq_t)7 * nn * nn - (seq_t)17 * nn + (seq_t)30) / (seq_t)2);

    /* Return the result */
    return pow3 - q;
}
