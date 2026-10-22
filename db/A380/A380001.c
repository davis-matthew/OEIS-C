/*
 * A380001  Fixed points of A378299.
 * https://oeis.org/A380001
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0) = 0; a(1) = 1; a(n) = 8*a(n-1) + 5 for n > 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t val = (seq_t)1;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        val = (seq_t)8 * val + (seq_t)5;
    }
    return val;
}
