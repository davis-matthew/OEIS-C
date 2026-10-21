/*
 * A277547  a(n) = n/9^m mod 9, where 9^m is the greatest power of 9 that
 *          divides n.
 * https://oeis.org/A277547
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n/9^m mod 9 where 9^m is the greatest power of 9 dividing
 *            n
 * Range:     u8 n <= 437, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t v = (seq_t)n;
    while (v % (seq_t)9 == 0)
    {
        v /= (seq_t)9;
    }
    return v % (seq_t)9;
}
