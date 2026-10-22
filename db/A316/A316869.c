/*
 * A316869  Number of times 8 appears in decimal expansion of n.
 * https://oeis.org/A316869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) counts the number of decimal digits equal to 8 in the
 *            decimal representation of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    idx_t t = n;
    while (t > 0) {
        if (t % 10 == 8)
            ++count;
        t /= 10;
    }
    return count;
}
