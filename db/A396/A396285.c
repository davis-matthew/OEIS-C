/*
 * A396285  a(n) is the number of distinct triples of triangles chosen from
 *          the n^2 unit triangles of the triangular grid up to rotation.
 * https://oeis.org/A396285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/3)*(binomial(n^2,3) + 2*floor(n^2/3))
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 65, u64 n <= 2631,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Calculate n^2 in an intermediate type big enough to avoid overflow */
    unsigned __int128 n2 = (unsigned __int128)n * n;

    /* binomial(n^2,3) = n^2 * (n^2-1) * (n^2-2) / 6   */
    unsigned __int128 binom = n2 * (n2 - 1) * (n2 - 2) / 6;

    /* floor(n^2/3) */
    unsigned __int128 floor3 = n2 / 3;

    /* Final result: (binom + 2*floor3) / 3  */
    unsigned __int128 res = (binom + 2 * floor3) / 3;

    return (seq_t)res;   /* cast back to seq_t; safe for values that fit */
}
