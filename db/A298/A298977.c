/*
 * A298977  Base-7 complementary numbers: n equals the product of the 7
 *          complement (7-d) of its base-7 digits d.
 * https://oeis.org/A298977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1) = 12 a(2k) = 12·7^k for k ≥ 1 a(2k+1) = 120·7^k for k ≥ 1
 *            (equivalently a(n+2) = 7·a(n) for n ≥ 2)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* undefined for n < 1, just a safety guard */

    if (n == 1) return (seq_t)12;

    seq_t pow = (seq_t)1;
    idx_t exp;

    if ((n & 1) == 0) /* even n: n = 2k */
    {
        exp = n >> 1;           /* k = n/2 */
        for (idx_t i = 0; i < exp; ++i) pow *= (seq_t)7;
        return (seq_t)12 * pow;
    }
    else /* odd n: n = 2k+1, k ≥ 1 */
    {
        exp = (n - 3) >> 1;     /* k = (n-3)/2 */
        for (idx_t i = 0; i < exp; ++i) pow *= (seq_t)7;
        return (seq_t)120 * pow;
    }
}
