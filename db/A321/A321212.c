/*
 * A321212  Numbers that are congruent to {2, 3} mod 16.
 * https://oeis.org/A321212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (16*n - 7*(-1)^n - 19)/2. Equivalent: a(n) = (8*n - 6) if
 *            n is odd, a(n) = (8*n - 13) if n is even
 * Range:     u8 n <= 32, u16 n <= 8192, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based. It is guaranteed that n >= 1. */
    if (n & 1) {
        /* n odd: 8*n - 6 */
        seq_t res = ((seq_t)8 * n) - 6;
        return res;
    } else {
        /* n even: 8*n - 13 */
        seq_t res = ((seq_t)8 * n) - 13;
        return res;
    }
}
