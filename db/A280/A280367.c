/*
 * A280367  Binary representation of the x-axis, from the left edge to the
 *          origin, of the n-th stage of growth of the two-dimensional
 *          cellular automaton defined by "Rule 259", based on the 5-celled
 *          von Neumann neighborhood.
 * https://oeis.org/A280367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 when n = 0 or 1; a(n) = 11 when n is even and n > 1;
 *            a(n) = the decimal number whose digit string consists of (n‑2)
 *            '1' digits followed by "000" when n is odd and n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle trivial cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* even indices > 1 */
    if ((n & (idx_t)1) == 0)
        return (seq_t)11;

    /* odd indices > 1 : build the decimal number n‑2 ones followed by 000 */
    seq_t result = (seq_t)0;
    /* append n‑2 '1' digits */
    for (idx_t i = 0; i < n - 2; ++i)
        result = result * (seq_t)10 + (seq_t)1;

    /* append three '0' digits */
    for (int j = 0; j < 3; ++j)
        result = result * (seq_t)10;

    return result;
}
