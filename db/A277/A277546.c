/*
 * A277546  a(n) = n/8^m mod 8, where 8^m is the greatest power of 8 that
 *          divides n.
 * https://oeis.org/A277546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n / 8^m mod 8, where 8^m is the greatest power of 8 that
 *            divides n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the problem statement; guard against 0 just in case. */
    if (n == 0) return 0;

    seq_t t = (seq_t)n;
    while ((t % 8) == 0)
        t /= 8;
    return t % 8;
}
