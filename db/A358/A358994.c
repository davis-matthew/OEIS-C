/*
 * A358994  The sum of the numbers that are inside the contour of an n-story
 *          Christmas tree drawn at the top of the numerical pyramid
 *          containing the positive integers in natural order.
 * https://oeis.org/A358994
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(27*n^3 + 66*n^2 + 49*n + 26) / 8
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 187, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1 – value outside the range is irrelevant */
    seq_t nn = (seq_t)n;
    seq_t term = (seq_t)27 * nn * nn * nn
               + (seq_t)66 * nn * nn
               + (seq_t)49 * nn
               + (seq_t)26;
    seq_t val = nn * term;
    val /= (seq_t)8;
    return val;
}
