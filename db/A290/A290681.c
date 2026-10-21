/*
 * A290681  Binary representation of the diagonal from the origin to the
 *          corner of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 929", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A290681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^(n+1)-1)/9 for n>=4; a(0)=1, a(1)=1, a(2)=111,
 *            a(3)=111
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2 || n == 3) return 111;

    seq_t result = 0;
    for (idx_t i = 0; i < n + 1; ++i) {
        result = result * 10 + 1;
    }
    return result;
}
