/*
 * A285473  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 3", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is even, otherwise a(n) = (10^{n+1} - 1)/9, i.e.
 *            the decimal number consisting of (n + 1) consecutive 1's
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return (seq_t)1;

    /* Compute a decimal number with (n + 1) digits all equal to 1 */
    idx_t k = n + 1;          /* number of digits */
    seq_t res = 0;
    for (idx_t i = 0; i < k; ++i)
        res = res * ((seq_t)10) + ((seq_t)1);

    return res;
}
