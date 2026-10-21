/*
 * A295771  a(n) is the minimum size of a planar additive basis for the square
 *          [0,n]^2.
 * https://oeis.org/A295771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 if n == 0 * a(n) = 2*n if n is even (n > 0) * a(n) =
 *            2*n if n is odd, n % 6 == 1, and n >= 7 * a(n) = 2*n + 1
 *            otherwise
 * Range:     u8 n <= 13, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special case n = 0 */
    if (n == 0ULL) {
        return (seq_t)1;
    }

    /* base value 2*n computed in seq_t */
    seq_t two_n = (seq_t)n + (seq_t)n;

    /* even n (except n == 0 already handled) */
    if ((n & 1ULL) == 0ULL) {
        return two_n;
    }

    /* odd n: check n % 6 == 1 and n >= 7 */
    if ((n % 6ULL) == 1ULL && n >= 7ULL) {
        return two_n;
    }

    /* default case for odd n */
    return two_n + (seq_t)1;
}
