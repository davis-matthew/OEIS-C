/*
 * A259319  a(n) = 2*A002309(n).
 * https://oeis.org/A259319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n*(7-40*n^2+48*n^4))/15
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to avoid accidental promotion */
    seq_t n_seq = n;
    /* Compute powers of n */
    seq_t n2   = n_seq * n_seq;      /* n^2 */
    seq_t n4   = n2   * n2;          /* n^4 */
    /* Inner polynomial: 7 - 40*n^2 + 48*n^4 */
    seq_t inner = (seq_t)7 + (seq_t)48 * n4 - (seq_t)40 * n2;
    /* Numerator: 2 * n * inner */
    seq_t numerator = (seq_t)2 * n_seq * inner;
    /* Division by 15 is exact */
    return numerator / (seq_t)15;
}
