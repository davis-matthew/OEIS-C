/*
 * A301654  Circumference of the n-triangular honeycomb acute knight graph.
 * https://oeis.org/A301654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)*(n-1)/6)
 * Range:     u8 n <= 15, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1, so (n-1) is non‑negative. Use seq_t arithmetic to avoid
       overflow on intermediate results when seq_t has larger range. */
    seq_t nn = (seq_t)n;
    seq_t num = (nn + 2) * (nn - 1);   /* (n+2)*(n-1) */
    return num / (seq_t)6;            /* floor division for positive numbers */
}
