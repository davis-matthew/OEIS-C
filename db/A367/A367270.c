/*
 * A367270  Triangle read by rows. T(n, k) = binomial(n, k)*binomial(n - 1, n
 *          - k - 1).
 * https://oeis.org/A367270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where (r,k) is the row and column of the triangle
 *            entry at flattened index n, and T(r,k) = binomial(r,k) *
 *            binomial(r-1, r-k-1)
 * Range:     u8 n <= 29, u16 n <= 69, u32 n <= 198, u64 n <= 681,
 *            u128 n <= 2378, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial coefficient C(n,k) for small n (n <= row index).  */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return (seq_t)0;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= k; ++i) {
        seq_t numerator   = (seq_t)(n - k + i);
        seq_t denominator = (seq_t)i;
        res = (res * numerator) / denominator;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t r = (idx_t)0;
    while ( ((__int128)(r + (idx_t)1) * (r + (idx_t)2)) / (idx_t)2 <= n ) {
        ++r;
    }

    /* Position within the row  */
    idx_t prefix = ((idx_t)r * (r + (idx_t)1)) / (idx_t)2;
    idx_t k = n - prefix;

    if (r == (idx_t)0)          /* first row, single entry */
        return (seq_t)1;
    if (k == r)                 /* last entry of a row is zero */
        return (seq_t)0;

    /* Compute the product of the two binomials */
    seq_t bin1 = binom(r, k);
    seq_t bin2 = binom(r - (idx_t)1, r - k - (idx_t)1);
    return bin1 * bin2;
}
