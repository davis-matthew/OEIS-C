/*
 * A316866  Number of times 5 appears in decimal expansion of n.
 * https://oeis.org/A316866
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of times the digit 5 occurs in the decimal
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t tmp = n;
    while (tmp != 0) {
        if (tmp % 10 == 5)
            ++count;
        tmp /= 10;
    }
    return count;
}
