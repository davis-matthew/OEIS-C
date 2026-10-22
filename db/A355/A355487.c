/*
 * A355487  Bitwise XOR of the base-4 digits of n.
 * https://oeis.org/A355487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = XOR of base-4 digits of n
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t t = n;
    while (t) {
        res ^= (seq_t)(t & 3);  /* base-4 digit: lower 2 bits */
        t >>= 2;                /* next base-4 digit */
    }
    return res;
}
