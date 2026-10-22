/*
 * A389928  Truncated centered square numbers: a(n) = 14*n^2 - 22*n + 9.
 * https://oeis.org/A389928
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*n^2 - 22*n + 9
 * Range:     u8 n <= 5, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence type */
    seq_t tn = (seq_t)n;

    /* Compute the formula using unsigned arithmetic */
    seq_t term = (seq_t)14 * tn * tn;  /* 14*n^2 */
    term -= (seq_t)22 * tn;            /* -22*n */
    term += (seq_t)9;                 /* +9 */

    return term;
}
