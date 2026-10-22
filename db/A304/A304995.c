/*
 * A304995  Expansion of g.f. (1 + 6*x + 6*x^2 + 6*x^3 + x^4 + 6*x^5)/((1 -
 *          x)*(1 + x^4)).
 * https://oeis.org/A304995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; for n>0, a(n)=pattern[(n-1)%8] with pattern =
 *            {7,13,19,19,19,13,7,7}
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first term separately */
    if (n == 0u)
        return (seq_t)1;

    /* Pattern repeats every 8 terms starting from n = 1 */
    static const seq_t pattern[8] = {7, 13, 19, 19, 19, 13, 7, 7};

    /* Compute (n-1) mod 8 efficiently using bitwise AND (since 8 is 2^3) */
    idx_t idx = (n - 1u) & 7u;      /* idx in [0,7] */

    return pattern[idx];
}
