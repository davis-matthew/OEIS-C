/*
 * A283641  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 678", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A283641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the decimal integer whose base‑10 digits are the * binary
 *            pattern along the x–axis of the n‑th stage of Rule 678 CA. * If
 *            n is even: digits = 1 followed by n/2 copies of 01, ending with
 *            1. * If n is odd: digits = 11 followed by (n-1)/2 copies of 01.
 *            * (Sequence A283641)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)0;
    if ((n & 1ULL) == 0) {               /* even n */
        for (idx_t j = 0; j <= n; ++j) {
            res = res * (seq_t)10 + ( (j & 1ULL) ? (seq_t)0 : (seq_t)1 );
        }
    } else {                              /* odd n */
        for (idx_t j = 0; j <= n; ++j) {
            seq_t digit;
            if (j == 0 || j == 1) {
                digit = (seq_t)1;
            } else {
                /* after the first two 1s, alternate 0,1 starting with 0 */
                digit = ( ( (j - 2) & 1ULL ) ? (seq_t)1 : (seq_t)0 );
            }
            res = res * (seq_t)10 + digit;
        }
    }
    return res;
}
