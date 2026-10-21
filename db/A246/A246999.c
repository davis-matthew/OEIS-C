/*
 * A246999  a(n) is the binary word s21s211s2 where s is a string of n 1's.
 * https://oeis.org/A246999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1^n 21 1^n 211 1^n 2 (i.e., concatenate n copies of '1',
 *            then "21", then n copies of '1', then "211", then n copies of
 *            '1', then "2" – interpreted as a decimal integer)
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 4, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;

    /* first block: n copies of '1' */
    for (idx_t i = 0; i < n; ++i)
        res = res * 10 + 1;

    /* middle pattern "21" */
    res = res * 10 + 2;
    res = res * 10 + 1;

    /* second block: n copies of '1' */
    for (idx_t i = 0; i < n; ++i)
        res = res * 10 + 1;

    /* middle pattern "211" */
    res = res * 10 + 2;
    res = res * 10 + 1;
    res = res * 10 + 1;

    /* third block: n copies of '1' */
    for (idx_t i = 0; i < n; ++i)
        res = res * 10 + 1;

    /* final '2' */
    res = res * 10 + 2;

    return res;
}
