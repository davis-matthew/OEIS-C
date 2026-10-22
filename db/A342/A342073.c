/*
 * A342073  Number of n-colorings of the vertices of the 5-dimensional cross
 *          polytope such that no two adjacent vertices have the same color.
 * https://oeis.org/A342073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for 0 ≤ n ≤ 4; a(n) = (n-4)*(n-3)*(n-2)*(n-1)*n*(-8544
 *            + 6909*n - 2240*n^2 + 365*n^3 - 30*n^4 + n^5) for n ≥ 5
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 13, u64 n <= 88, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t prod = nn;
    prod *= nn - 1;
    prod *= nn - 2;
    prod *= nn - 3;
    prod *= nn - 4;

    /* compute powers of n */
    seq_t n1 = nn;
    seq_t n2 = n1 * n1;
    seq_t n3 = n2 * n1;
    seq_t n4 = n3 * n1;
    seq_t n5 = n4 * n1;

    /* positive part: 6909*n + 365*n³ + n⁵ */
    seq_t pos = (seq_t)6909 * n1;
    pos += (seq_t)365 * n3;
    pos += n5;

    /* negative part: 8544 + 2240*n² + 30*n⁴ */
    seq_t neg = (seq_t)8544;
    neg += (seq_t)2240 * n2;
    neg += (seq_t)30 * n4;

    seq_t poly = pos - neg;
    return prod * poly;
}
