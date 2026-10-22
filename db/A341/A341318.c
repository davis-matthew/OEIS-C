/*
 * A341318  Lower triangular table of products in the semigroup S = {(0,0),
 *          (i,j): i >= j >= 1} (see Comments for precise definition), read by
 *          rows.
 * https://oeis.org/A341318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = label of the product of the element with index row =
 *            ⌊(√(8n+1)-1)/2⌋ and the element with index col = n -
 *            row*(row+1)/2. The element with index k>0 corresponds to the
 *            pair (i,j) where i = ⌈(√(1+8k)-1)/2⌉ j = k - (i-1)*i/2. The
 *            product of (i1,j1) and (i2,j2) is (i1*i2, j1*j2), whose label
 *            i...
 * Range:     u8 n <= 76, u16 n <= 9874, u32 n <= 9874, u64 n <= 9874,
 *            u128 n <= 9874, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper: integer square root for __int128 values */
static inline uint64_t isqrt128(__int128 x)
{
    uint64_t low = 0;
    uint64_t high = (uint64_t)1 << 34;   /* 2^34 > sqrt(8*UINT64_MAX+1) */
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Convert element index to its pair (i,j) */
static inline void idx_pair(idx_t idx, idx_t *i, idx_t *j)
{
    if (idx == 0) { *i = 0; *j = 0; return; }

    uint64_t s = isqrt128(((__int128)8 * idx + 1));
    idx_t cand = (s - 1) / 2;
    if (((__int128)cand * (cand + 1)) / 2 < idx) cand++;

    *i = cand;
    idx_t start_prev = ( (__int128)(cand - 1) * cand ) / 2;
    *j = idx - start_prev;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    /* Compute row and column in the triangle */
    idx_t row = (isqrt128(((__int128)8 * n + 1)) - 1) / 2;
    idx_t row_start = ( (__int128)row * (row + 1) ) / 2;
    idx_t col = n - row_start;

    /* Get the pairs for the two indices */
    idx_t i1, j1, i2, j2;
    idx_pair(row, &i1, &j1);
    idx_pair(col, &i2, &j2);

    /* Compute the product (I,J) */
    unsigned __int128 I = (unsigned __int128)i1 * i2;
    unsigned __int128 J = (unsigned __int128)j1 * j2;

    /* Label of the product */
    unsigned __int128 label128 = ((I - 1) * I) / 2 + J;

    return (seq_t)label128;
}
