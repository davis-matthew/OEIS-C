/*
 * A294270  Sum of the cubes of the parts in the partitions of n into two
 *          parts.
 * https://oeis.org/A294270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (4*n^2 - 7*n + 4 + n*(-1)^n) / 16, n>=1
 * Range:     u8 n <= 6, u16 n <= 23, u32 n <= 215, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;                /* n^2 */
    seq_t term;
    if ((n & 1ULL) == 0) {             /* n even */
        term = (seq_t)4 * n2 - (seq_t)7 * nn + (seq_t)4 + nn;
    } else {                           /* n odd  */
        term = (seq_t)4 * n2 - (seq_t)7 * nn + (seq_t)4 - nn;
    }
    return n2 * term / 16;             /* integer division is exact */
}
