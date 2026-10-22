/*
 * A358012  Minimal number of coins needed to pay n cents using coins of
 *          denominations 1 and 5 cents.
 * https://oeis.org/A358012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/5)+n%5
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)(n / 5) + (seq_t)(n % 5);
}
