/*
 * A368417  Binary numbers B whose binary expansion can be split into parts s
 *          and t where B = s^2 + t^2.
 * https://oeis.org/A368417
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 101 a(2*n) = decimal digits 1 repeated n times, 0
 *            repeated n times, 1, 0 repeated 2*n times (n > 0) a(2*n+1) =
 *            decimal digits (1 followed by (n-1) zeros) repeated twice, 0, 1,
 *            and 0 repeated 2*n times (n > 0) The function builds the decimal
 *            number with those digits by repeated multiplicati...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)101u;          /* 101₁₀ */

    seq_t res = 0;

    if ((n & 1ULL) == 0) {           /* n = 2*m,  m > 0 */
        idx_t m = n >> 1;            /* number of 1's and 0's in the first two parts */

        /* m ones */
        for (idx_t i = 0; i < m; ++i)
            res = res * 10 + 1;

        /* m zeros */
        for (idx_t i = 0; i < m; ++i)
            res *= 10;

        /* one */
        res = res * 10 + 1;

        /* 2*m zeros */
        for (idx_t i = 0; i < 2 * m; ++i)
            res *= 10;
    } else {                          /* n = 2*m+1,  m > 0 */
        idx_t m = (n - 1) >> 1;       /* parameter used in the pattern */

        /* first block: 1 followed by (m-1) zeros */
        res = res * 10 + 1;
        for (idx_t i = 0; i < m - 1; ++i)
            res *= 10;

        /* second block: same pattern again */
        res = res * 10 + 1;
        for (idx_t i = 0; i < m - 1; ++i)
            res *= 10;

        /* separate zero after the two blocks */
        res *= 10;

        /* the middle one */
        res = res * 10 + 1;

        /* trailing 2*m zeros */
        for (idx_t i = 0; i < 2 * m; ++i)
            res *= 10;
    }

    return res;
}
