/*
 * A317313  Multiples of 13 and odd numbers interleaved.
 * https://oeis.org/A317313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 13*n, a(2n+1) = 2*n+1
 * Range:     u8 n <= 39, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1) {
        return (seq_t)n;
    } else {
        return 13 * (seq_t)(n / 2);
    }
}
