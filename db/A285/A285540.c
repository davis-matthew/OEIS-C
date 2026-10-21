/*
 * A285540  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 35", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A285540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the decimal integer consisting of k ones, where
   k = n+1 if n is odd, otherwise k = n/2+1. */
seq_t a(idx_t n)
{
    /* compute number of ones */
    idx_t k = (n & 1) ? (n + 1) : (n / 2 + 1);

    /* build the integer of k ones */
    seq_t result = 0;
    for (idx_t i = 0; i < k; ++i)
        result = result * (seq_t)10 + (seq_t)1;

    return result;
}
