/*
 * A259834  Number of permutations of [n] with no fixed points where the
 *          maximal displacement of an element equals n-1.
 * https://oeis.org/A259834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = !(n-2) + 2*!(n-1) for n ≥ 2 where !(0) = 1, !(1) = 0
 *            (subfactorial). a(0) = 0, a(1) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset terms directly. */
    if (n <= 1)
        return (seq_t)0;

    /* Compute subfactorials iteratively to avoid extra storage. */
    seq_t s0 = (seq_t)1;   /* !(0) */
    seq_t s1 = (seq_t)0;   /* !(1) */
    seq_t s;              /* current subfactorial */

    /* For k = 2 … n-1 compute !(k) = (k-1)*(!(k-1) + !(k-2)). */
    for (idx_t k = 2; k <= n - 1; ++k)
    {
        s = (seq_t)(k - 1) * (s1 + s0);
        s0 = s1;
        s1 = s;
    }

    /* At this point: s1 = !(n-1), s0 = !(n-2).          */
    return s0 + (seq_t)2 * s1;
}
