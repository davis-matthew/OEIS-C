/*
 * A396835  Upper (2/3,1/3) midsequence of (floor[n/2]^2) and
 *          (ceiling[n/2]^2); see Comments.
 * https://oeis.org/A396835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((6*n^2 - 2*n + 15 + (-1)^n*(2*n - 7))/24)
 * Range:     u8 n <= 6, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to seq_t for the arithmetic */
    seq_t nn = (seq_t)n;
    seq_t num;

    /* Use the even/odd split to avoid dealing with negative values
       when seq_t is an unsigned type. */
    if ((n & 1ULL) == 0) {
        /* n even: numerator = 6*n^2 + 8 */
        num = 6 * nn * nn + (seq_t)8;
    } else {
        /* n odd: numerator = 6*n^2 - 4*n + 22 */
        num = 6 * nn * nn - 4 * nn + (seq_t)22;
    }

    /* integer division truncates toward zero, which is floor for non‑negative numerators */
    return num / (seq_t)24;
}
