/*
 * A298372  a(n), in decimal base, is the number of numbers k >= 0 with no
 *          more digits than n such that k + n can be computed without carry.
 * https://oeis.org/A298372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of (10 - digit) over decimal digits of n. For n =
 *            0, a(0) = 1
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t result = 1;
    while (n != 0)
    {
        unsigned d = (unsigned)(n % 10);
        result *= (seq_t)(10 - d);
        n /= 10;
    }
    return result;
}
