/*
 * A259486  a(n) = 3*n^2 - 3*n + 1 + 6*floor((n-1)*(n-2)/6).
 * https://oeis.org/A259486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 - 6*n + (n mod 3 == 0 ? 1 : 3)
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t to keep intermediate arithmetic exact */
    seq_t nn = (seq_t)n;

    /* n^2 as seq_t */
    seq_t val = nn * nn;

    /* 4*n^2 - 6*n */
    val = val * 4 - (seq_t)6 * nn;

    /* Add the constant depending on n mod 3 */
    if ((n % 3) == 0)
        val += (seq_t)1;
    else
        val += (seq_t)3;

    return val;
}
