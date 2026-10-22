/*
 * A353097  a(1) = 5; for n > 1, a(n) = 6*a(n-1) + 6 - n.
 * https://oeis.org/A353097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 6 - n (OEIS A353097)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for indices below the offset */

    seq_t v = (seq_t)5; /* a(1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        v = (seq_t)6 * v + (seq_t)6 - (seq_t)i;
    }

    return v;
}
