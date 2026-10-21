/*
 * A292350  Number of Lyndon words (aperiodic necklaces) with 6 beads of n
 *          colors.
 * https://oeis.org/A292350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 - n^3 - n^2 + n)/6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 48, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For safety handle n == 0. */
    if (n == 0) return 0;

    seq_t nn = (seq_t)n;        /* n as seq_t */
    seq_t n2 = nn * nn;         /* n^2 */
    seq_t n3 = n2 * nn;         /* n^3 */
    seq_t n6 = n3 * n3;         /* n^6 */

    seq_t val = n6 - n3 - n2 + nn;  /* n^6 - n^3 - n^2 + n */
    return val / 6;                 /* division is exact */
}
