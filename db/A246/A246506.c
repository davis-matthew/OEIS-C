/*
 * A246506  a(n) is the number m_0 with the property that if m >= m_0, then
 *          every graph obtained from the complete bipartite graph K_{m,m+n}
 *          by deleting two edges is chromatically unique.
 * https://oeis.org/A246506
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 5 + floor((n-1)/2) * ceil((n-1)/2) for n ≥ 5, a(n) = 7
 *            for n ≤ 4. (OEIS offset 2)
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the small n values directly. */
    if (n <= 4u)
        return 7;

    /* Compute floor((n-1)/2) and ceil((n-1)/2). */
    seq_t m          = (seq_t)(n - 1u);  /* m = n-1 */
    seq_t floor_half = m / 2;            /* integer division gives floor */
    seq_t ceil_half  = m - floor_half;   /* remaining part gives ceil */

    /* Product of the two halves gives the needed increment. */
    seq_t product = floor_half * ceil_half;

    /* Final result. */
    return (seq_t)5 + product;
}
