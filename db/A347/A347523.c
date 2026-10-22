/*
 * A347523  Characteristic function of nonpowers of 2.
 * https://oeis.org/A347523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is not a power of two (including n = 0), otherwise
 *            0
 * Range:     u8 n <= 9335, u16 n <= 9335, u32 n <= 9335, u64 n <= 9335,
 *            u128 n <= 9335, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is a power of two when n > 0 and n & (n-1) == 0 */
    if (n && ((n & (n - 1)) == 0))
        return (seq_t)0;   /* 2^k  -> 0 */
    else
        return (seq_t)1;   /* all other n          -> 1 */
}
