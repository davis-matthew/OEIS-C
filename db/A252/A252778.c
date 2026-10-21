/*
 * A252778  Number of ways of n-coloring the square grid graph G_(3,3) such
 *          that no rectangle exists with sides parallel to the axes having
 *          all 4 corners of the same color.
 * https://oeis.org/A252778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n^7+n^6+n^5-8*n^4-8*n^3+4*n^2+22*n-14)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;          /* a(0)=0, a(1)=0 */

    seq_t N   = (seq_t)n;
    seq_t Nm1 = N - (seq_t)1;

    /* Compute powers of N up to N^7 */
    seq_t n2 = N * N;           /* N^2 */
    seq_t n3 = n2 * N;          /* N^3 */
    seq_t n4 = n3 * N;          /* N^4 */
    seq_t n5 = n4 * N;          /* N^5 */
    seq_t n6 = n5 * N;          /* N^6 */
    seq_t n7 = n6 * N;          /* N^7 */

    /* Evaluate polynomial: N^7 + N^6 + N^5 - 8*N^4 - 8*N^3 + 4*N^2 + 22*N - 14 */
    seq_t poly = n7;
    poly += n6;
    poly += n5;
    poly -= (seq_t)8 * n4;
    poly -= (seq_t)8 * n3;
    poly += (seq_t)4 * n2;
    poly += (seq_t)22 * N;
    poly -= (seq_t)14;

    /* Result: N * (N-1) * poly */
    return N * Nm1 * poly;
}
