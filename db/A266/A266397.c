/*
 * A266397  Number of orbits of Aut(Z^7) as function of the infinity norm n of
 *          the representative lattice point of the orbit, when the
 *          cardinality of the orbit is equal to 26880.
 * https://oeis.org/A266397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3+3*n^2-37*n+30)/6 Equivalent factorization: a(n) =
 *            ((n-1)*(n-2)*(4*n+15))/6 Returns 0 for n = 1 or 2
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2u)
        return 0;
    seq_t nm1 = (seq_t)(n - 1u);   /* n-1 */
    seq_t nm2 = (seq_t)(n - 2u);   /* n-2 */
    seq_t t3  = 4u * (seq_t)n + 15u; /* 4*n + 15 */
    seq_t prod = nm1 * nm2;
    prod = prod * t3;
    return prod / 6u;
}
