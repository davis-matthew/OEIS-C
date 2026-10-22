/*
 * A317315  Multiples of 15 and odd numbers interleaved.
 * https://oeis.org/A317315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(2n) = 15*n, a(2n+1) = 2*n + 1
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n >> 1;            /* m = floor(n/2) */
    if ((n & 1) == 0) {
        return (seq_t)15 * (seq_t)m;
    } else {
        return (seq_t)2 * (seq_t)m + (seq_t)1;
    }
}
