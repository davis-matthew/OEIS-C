/*
 * A305267  a(n) = 68*2^n + 358.
 * https://oeis.org/A305267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 68*2^n + 358
 * Range:     u8 none, u16 n <= 9, u32 n <= 25, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t p = 1;          /* p = 2^n, computed by repeated doubling */
    while (n--) {         /* shift by 1 each time – always safe */
        p <<= 1;
    }
    return p * (seq_t)68 + (seq_t)358;
}
