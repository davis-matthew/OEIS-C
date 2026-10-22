/*
 * A329482  Interleave 1 - n + 3*n^2, 1 + 3*n*(1+n) for n >= 0.
 * https://oeis.org/A329482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5 + 3*(-1)^n - 2*(1 + (-1)^n)*n + 6*n^2) / 8
 * Range:     u8 n <= 15, u16 n <= 294, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use an equivalent form that avoids negative intermediate terms,
       which is safe for unsigned arithmetic on seq_t. */
    seq_t nn = (seq_t)n * (seq_t)n;   /* n^2  */
    if ((n & 1u) == 0) {              /* even n: 6*n^2 - 4*n + 8 */
        return ((seq_t)6 * nn - (seq_t)4 * n + (seq_t)8) / (seq_t)8;
    } else {                          /* odd n: 6*n^2 + 2 */
        return ((seq_t)6 * nn + (seq_t)2) / (seq_t)8;
    }
}
