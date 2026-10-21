/*
 * A249708  Number of length 2+3 0..n arrays with every four consecutive terms
 *          having the maximum of some two terms equal to the minimum of the
 *          remaining two terms.
 * https://oeis.org/A249708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 8*n^3 + 11*n^2 + 6*n + 2)/2
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A249708 – Number of length 2+3 0..n arrays with the specified property */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid intermediate truncation */
    seq_t nn = (seq_t)n;

    /* Compute successive powers of n */
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */
    seq_t n4 = n3 * nn;        /* n^4 */

    /* Numerator of the closed‑form expression */
    seq_t num = n4 + ((seq_t)8 * n3) + ((seq_t)11 * n2) + ((seq_t)6 * nn) + (seq_t)2;

    /* Integer division by 2 yields the exact value */
    return num / (seq_t)2;
}
