/*
 * A269895  Number of n X 1 0..6 arrays with some element plus some
 *          horizontally or vertically adjacent neighbor totalling six exactly
 *          once.
 * https://oeis.org/A269895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*6^(n-2)*(n-1) for n≥2; a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    /* compute 6^(n-2) by repeated multiplication */
    seq_t power = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        power *= (seq_t)6;

    /* a(n) = 7 * 6^(n-2) * (n-1) */
    seq_t result = (seq_t)7 * power;
    result *= (seq_t)(n - 1);
    return result;
}
