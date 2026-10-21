/*
 * A285775  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 89", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; * a(1)=10; * a(2)=1; * a(3)=1110; * a(n)=0 for even n>3;
 *            * a(n) = 101*a(n-2)-100*a(n-4) for odd n>7, * equivalently a(n)
 *            = (10^(n+1)-1)/9 for odd n>3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small special cases directly */
    if (n == 0) return 1;
    if (n == 1) return 10;
    if (n == 2) return 1;
    if (n == 3) return 1110;

    /* For n > 3: even indices produce zero */
    if ((n & 1) == 0) return 0;

    /* n is odd and >=5.  Produce a decimal number consisting of
     * (n+1) ones.  This equals (10^(n+1)-1)/9. */
    seq_t result = 0;
    for (idx_t i = 0; i < n + 1; ++i) {
        result = result * 10 + 1;
    }
    return result;
}
