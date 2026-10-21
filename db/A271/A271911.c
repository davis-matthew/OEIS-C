/*
 * A271911  Number of ways to choose three distinct points from a 2 X n grid
 *          so that they form an isosceles triangle.
 * https://oeis.org/A271911
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = round(n * (n+6) / 2) - 4 Equivalent integer form: a(n) =
 *            (n*(n+6) + (n & 1)) / 2 - 4
 * Range:     u8 n <= 13, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)            /* below the offset; not required but safe */
        return 0;

    seq_t nn   = (seq_t)n;            /* convert index to seq_t */
    seq_t prod = nn * (nn + 6);       /* n*(n+6)  */
    if (nn & 1)                      /* if n odd, round up */
        prod += 1;
    seq_t res = prod / 2;            /* divide by 2 with rounding */
    res -= 4;                        /* final adjustment */
    return res;
}
