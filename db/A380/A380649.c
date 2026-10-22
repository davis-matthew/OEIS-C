/*
 * A380649  Rectangular array ((-1)*D(i,j,1,2)) read by descending
 *          antidiagonals: D(i,j,s,n) denotes the determinant of the matrix
 *          described in Comments.
 * https://oeis.org/A380649
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3k-1 + (k-1)(k-2)/2 - ( n - k(k-1)/2 ) where k is the
 *            smallest integer with k(k+1)/2 >= n
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute triangular number T(k) = k(k+1)/2 using 128–bit arithmetic */
static inline idx_t triangular(idx_t k)
{
    return (idx_t)(((__uint128_t)k * (k + 1)) >> 1);
}

seq_t a(idx_t n)
{
    /* Find k such that T(k-1) < n <= T(k) */
    idx_t k = 1;
    while (triangular(k) < n)
        ++k;

    /* i = n - T(k-1)  (row index on the k‑th antidiagonal) */
    idx_t i = n - triangular(k - 1);

    /* part1 = 3k - 1 */
    idx_t part1 = 3 * k - 1;

    /* part2 = (k-1)(k-2)/2 , zero when k <= 1 */
    idx_t part2 = 0;
    if (k > 1)
        part2 = (idx_t)(((__uint128_t)(k - 1) * (k - 2)) >> 1);

    /* a(n) = part1 + part2 - i */
    return (seq_t)(part1 + part2 - i);
}
