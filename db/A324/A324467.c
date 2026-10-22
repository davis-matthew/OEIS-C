/*
 * A324467  Three times the binary weight of n: 3*A000120(n).
 * https://oeis.org/A324467
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*A000120(n) (three times binary weight of n)
 * Range:     u8 n <= 8191, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;
    unsigned int ones = 0;
    while (x) {
        ones += (unsigned int)(x & 1);
        x >>= 1;
    }
    return (seq_t)3 * (seq_t)ones;
}
