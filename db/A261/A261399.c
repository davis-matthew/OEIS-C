/*
 * A261399  a(n) = (2/5)*(9*6^(n-2)+1).
 * https://oeis.org/A261399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 2, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first term explicitly. */
    if (n == 1)
        return 1;

    /* Compute the terms iteratively.  This works for all
       seq_t types (fixed-width integers or arbitrary-precision). */
    seq_t val = 1;
    for (idx_t i = 2; i <= n; ++i) {
        val = val * 6 - 2;
    }
    return val;
}
