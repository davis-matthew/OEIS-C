/*
 * A268643  Number of 1's in decimal representation of n.
 * https://oeis.org/A268643
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of digit '1' in decimal representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    while (n != 0) {
        if (n % 10 == 1)
            ++count;
        n /= 10;
    }
    return count;
}
