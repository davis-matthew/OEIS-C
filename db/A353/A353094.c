/*
 * A353094  a(1) = 2; for n > 1, a(n) = 3*a(n-1) + 3 - n.
 * https://oeis.org/A353094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 3 - n, a(1) = 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since the offset is 1, treat n >= 1.  For n < 1 the
       result is undefined; we return 0 in that case for safety. */
    if (n == 0)
        return 0;

    seq_t cur = 2;              /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = cur * 3 + 3 - (seq_t)i;
    }
    return cur;
}
