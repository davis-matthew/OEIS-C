/*
 * A276394  Characteristic word associated with the fraction 36/25.
 * https://oeis.org/A276394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+2)*36/25) - floor((n+1)*36/25) - 1
 * Range:     u8 n <= 147, u16 n <= 147, u32 n <= 147, u64 n <= 147,
 *            u128 n <= 147, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the two Beatty sequence terms and take the difference. */
    seq_t t1 = (seq_t)(n + 2) * 36 / 25;
    seq_t t2 = (seq_t)(n + 1) * 36 / 25;
    return t1 - t2 - 1;
}
