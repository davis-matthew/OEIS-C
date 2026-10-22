/*
 * A390764  Number of digits in base-6 representation of n.
 * https://oeis.org/A390764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(log_6(n)) + 1, with a(0) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 1;            /* at least one digit (including a(0)) */
    idx_t v = n;
    while (v >= 6)
    {
        v /= 6;
        ++count;
    }
    return count;
}
