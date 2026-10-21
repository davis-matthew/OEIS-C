/*
 * A286521  Decimal representation of the diagonal from the origin to the
 *          corner (or of the corner to the origin) of the n-th stage of
 *          growth of the two-dimensional cellular automaton defined by "Rule
 *          659", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A286521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=5, a(n)=2^(n+1)-1 for n>=3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle small indices directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)5;

    /* For n >= 3 compute 2^(n+1) - 1 by repeated multiplication.
       This works for arbitrary-precision seq_t and is fast enough
       for n up to at least 10000. */
    seq_t result = (seq_t)1;
    idx_t exp = n + 1;          /* exponent = n + 1 */
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)2;     /* multiply by 2 each iteration */

    return result - (seq_t)1;
}
