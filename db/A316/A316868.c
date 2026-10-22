/*
 * A316868  Number of times 7 appears in decimal expansion of n.
 * https://oeis.org/A316868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = number of digit 7 in decimal representation of n
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) {
        return 0;
    }
    for (idx_t tmp = n; tmp > 0; tmp /= 10) {
        if (tmp % 10 == 7) {
            ++count;
        }
    }
    return count;
}
