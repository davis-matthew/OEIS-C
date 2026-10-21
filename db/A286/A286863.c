/*
 * A286863  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 814", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A286863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^k - 1)/9, where k = 1 << (floor(log₂(n)) - 1) for n ≥
 *            1, and k = 1 for n = 0 (a(0)=1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special case n = 0. */
    if (n == 0)
        return (seq_t)1;

    /* Compute floor(log₂(n)). */
    idx_t t = 0;
    idx_t tmp = n;
    while ((tmp >> 1) != 0) { /* while tmp ≥ 2 */
        tmp >>= 1;
        ++t;
    }
    /* t = ⌊log₂(n)⌋ ≥ 1 here. */

    /* Determine the number of decimal ones (k). */
    idx_t k = 1;
    if (t > 0)
        k <<= (t - 1);   /* k = 2^(t-1) */

    /* Construct the integer consisting of k ones in decimal. */
    seq_t val = 0;
    for (idx_t i = 0; i < k; ++i)
        val = val * 10 + 1;

    return val;
}
