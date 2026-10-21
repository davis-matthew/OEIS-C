/*
 * A285326  a(0) = 0, for n > 0, a(n) = n + A006519(n).
 * https://oeis.org/A285326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n + (n & -n) (i.e., n plus the largest power of 2
 *            dividing n)
 * Range:     u8 n <= 127, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    /* Convert index to the sequence's integer type */
    seq_t x = (seq_t)n;
    /* Compute the lowest set bit (largest power of two dividing n) */
    seq_t lowbit = x & ((~x) + 1);
    /* Return n plus that lowest set bit */
    return x + lowbit;
}
