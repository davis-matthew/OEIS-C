/*
 * A317311  Multiples of 11 and odd numbers interleaved.
 * https://oeis.org/A317311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(2n) = 11*n; a(2n+1) = 2*n+1
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1u) == 0) {
        /* n is even: n = 2k, result = 11*k */
        return 11 * ((seq_t)(n >> 1));
    } else {
        /* n is odd: n = 2k+1, result = 2*k+1 */
        return 2 * ((seq_t)((n - 1) >> 1)) + 1;
    }
}
