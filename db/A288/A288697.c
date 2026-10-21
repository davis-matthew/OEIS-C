/*
 * A288697  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 494", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A288697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(1) = 1 a(2) = 0 For n >= 3: a(n) = 1 if n is odd a(n)
 *            = 10 if n is even (Values are the decimal integers whose decimal
 *            digits are the binary representation of the diagonal
 * Range:     u8 n <= 126, u16 n <= 126, u32 n <= 126, u64 n <= 126,
 *            u128 n <= 126, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)           /* n = 0 or 1 */
        return (seq_t)1;
    if (n == 2)
        return (seq_t)0;
    /* n >= 3 */
    return ((n & 1) == 0) ? (seq_t)10 : (seq_t)1;
}
