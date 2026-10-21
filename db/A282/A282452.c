/*
 * A282452  Binary representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 475", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A282452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10 for odd n > 0 * a(0) = 1 * a(n) = (10^(n+1) – 1) / 9
 *            for even n * (i.e. a decimal number composed of n + 1 repeated
 *            '1' digits)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case n = 0 */
    if (n == 0)
        return (seq_t)1;

    /* Odd n → constant decimal 10 */
    if (n & 1ULL)
        return (seq_t)10;

    /* Even n → build the decimal number n+1 ones */
    idx_t k = n + 1;          /* k = number of digits, fits into idx_t for n ≤ 10000 */
    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i)
        val = val * (seq_t)10 + (seq_t)1;

    return val;
}
