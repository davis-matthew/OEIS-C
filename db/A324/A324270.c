/*
 * A324270  a(n) = 13*7^(7*n).
 * https://oeis.org/A324270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 13 * 823543^n
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast binary exponentiation for arbitrary-precision integer base */
    seq_t base = 823543;          /* 7^7 */
    seq_t result = 1;             /* accumulator, seq_t type handles 1 */
    idx_t exp = n;                /* copy of the exponent */

    while (exp > 0) {
        if (exp & 1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }

    return result * 13;
}
