/*
 * A381490  Index of second half of decomposition of integers into pairs
 *          x(i)+y(j) based on A380008 and A380009, respectively.
 * https://oeis.org/A381490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n & 1) + 2 * ((n >> 3) & 1) + 4 * (n >> 5)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)(n & 1);
    result += 2 * (seq_t)((n >> 3) & 1);
    result += 4 * (seq_t)(n >> 5);
    return result;
}
