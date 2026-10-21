/*
 * A250388  Number of length 2+3 0..n arrays with no four consecutive terms
 *          having the maximum of any two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A250388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + (3/2)*n^4 + 2*n^3 + (3/2)*n^2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 74, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recursive version: a(n) = 6*a(n‑1) - 15*a(n‑2) + 20*a(n‑3) - 15*a(n‑4) + 6*a(n‑5) - a(n‑6) */
seq_t a(idx_t n)
{
    /* Compute using the explicit polynomial formula; this is O(1) */
    if (n == 0) return 0;          /* defined only for n>=1, but harmless */
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;            /* n² */
    seq_t n3 = n2 * sn;            /* n³ */
    seq_t n4 = n3 * sn;            /* n⁴ */
    seq_t n5 = n4 * sn;            /* n⁵ */

    /* a(n) = (2*n⁵ + 3*n⁴ + 4*n³ + 3*n²)/2 */
    return ((seq_t)2 * n5 + (seq_t)3 * n4 + (seq_t)4 * n3 + (seq_t)3 * n2) / 2;
}
