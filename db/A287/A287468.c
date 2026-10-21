/*
 * A287468  Binary representation of the diagonal from the corner to the
 *          origin of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 276", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A287468
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 10 * a(2(k-1)) + 1 (k ≥ 1) a(2k+1) = 0 with a(0) = 1
 *            (equivalently a(n) = (10^(n/2+1)-1)/9 for even n, a(n)=0 for odd
 *            n)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for odd indices */
    if (n & 1ULL)
        return 0;

    /* n is even; compute  a(2k) = 10 * a(2(k-1)) + 1 iteratively */
    seq_t res = 1;          /* a(0) */
    idx_t k = n >> 1;       /* k = n/2 */

    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * 10 + 1;
    }

    return res;
}
