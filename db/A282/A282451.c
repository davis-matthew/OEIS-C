/*
 * A282451  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 475", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A282451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = (10^(2k+1)-1)/9, a(2k+1) = 10^k
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is unsigned; treat even/odd cases separately */
    if ((n & 1U) == 0) {                /* even n */
        seq_t val = 0;
        /* produce a string of (n+1) digits '1' */
        for (idx_t i = 0; i <= n; ++i) {
            val = val * 10 + 1;
        }
        return val;
    } else {                            /* odd n */
        seq_t val = 1;
        /* produce a '1' followed by (n-1) zeros */
        for (idx_t i = 0; i < n - 1; ++i) {
            val *= 10;
        }
        return val;
    }
}
