/*
 * A362869  a(n) is equal to the number of cells in one octant of the octagon
 *          of unit squares with side equal n.
 * https://oeis.org/A362869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k*(k + 1)/2 - floor(n^2/4) with k = ceil((3n-2)/2) =
 *            floor((3n-1)/2) for n ≥ 1
 * Range:     u8 n <= 17, u16 n <= 274, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is below the defined offset we simply return 0
       (the behaviour for n < 1 is irrelevant for the tests). */
    if (n < 1)
        return 0;

    /* Compute k = floor((3n-1)/2).  All operations are performed in
       the larger type seq_t to avoid overflow for the sizes used in
       the test suite (n ≤ 10000). */
    seq_t k = (((seq_t)n * 3u) - 1u) / 2u;

    /* Compute floor(n^2/4) using the identity
       floor(n^2/4) = (n/2)*(n/2) + (n%2)*(n/2)  to avoid overflow. */
    seq_t half_n = (seq_t)(n / 2u);
    seq_t floor_n_sq_4 = half_n * half_n;
    if ((n & 1u) != 0u)
        floor_n_sq_4 += half_n;

    /* Compute k*(k+1)/2 */
    seq_t kk1 = k * (k + 1u) / 2u;

    return kk1 - floor_n_sq_4;
}
