/*
 * A286519  Binary representation of the diagonal from the origin to the
 *          corner (or of the corner to the origin) of the n-th stage of
 *          growth of the two-dimensional cellular automaton defined by "Rule
 *          659", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A286519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=11, a(2)=101, a(n) = (10^(n+1)-1)/9 for n≥3 (i.e.
 *            a(n) is a number consisting of n+1 decimal digits '1')
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)11;
    if (n == 2) return (seq_t)101;

    seq_t res = 0;
    /* Build a number with (n+1) ones by repeated mult–add. */
    for (idx_t i = 0; i <= n; ++i) {
        res = res * 10 + 1;
    }
    return res;
}
