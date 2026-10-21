/*
 * A276729  Number of nonprime digits in the decimal expansion of n.
 * https://oeis.org/A276729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of decimal digits of n that are not 2, 3, 5 or 7
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0)               /* decimal representation is "0" */
        return 1;
    while (n > 0) {
        idx_t d = n % 10;
        if (d != 2 && d != 3 && d != 5 && d != 7)
            count++;
        n /= 10;
    }
    return count;
}
