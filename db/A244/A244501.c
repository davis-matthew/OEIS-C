/*
 * A244501  Number of ways to place 3 points on an n X n X n triangular grid
 *          so that no pair of them has distance sqrt(3).
 * https://oeis.org/A244501
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/48*n^6 + 1/16*n^5 - 13/16*n^4 + 61/48*n^3 + 247/24*n^2
 *            - 293/6*n + 63 (for n >= 3)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 75, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the offset: a(2) = 1, values for n < 2 are unspecified */
    if (n == 2) return (seq_t)1;
    if (n < 2)  return (seq_t)0;   /* out of range, return 0 */

    /* Compute with integer arithmetic to avoid floating point */
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t n5 = n4 * N;
    seq_t n6 = n5 * N;

    seq_t num = n6
              + 3 * n5
              - 39 * n4
              + 61 * n3
              + 494 * n2
              - 2344 * N
              + 3024;

    return num / (seq_t)48;
}
