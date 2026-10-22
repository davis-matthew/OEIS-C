/*
 * A361226  Square array T(n,k) = k*((1+2*n)*k - 1)/2; n>=0, k>=0, read by
 *          antidiagonals upwards.
 * https://oeis.org/A361226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read the square array T(n,k) = k*((1+2*n)*k - 1)/2 by
 *            antidiagonals upwards. For antidiagonal d = n + k, values are
 *            listed with decreasing n (increasing k). The function finds the
 *            diagonal, the offset on it, computes n and k, and then evaluates
 *            T
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the largest d such that d*(d+1)/2 <= n.
       Binary search with uint128 intermediate to avoid overflow. */
    idx_t lo = 0, hi = 1;
    /* Expand hi until it exceeds the required bound. */
    while ( ( (__uint128_t)hi * (hi + 1) / 2 ) <= n ) hi <<= 1;
    /* Binary search in [lo, hi) */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo + 1) / 2;
        if ( ( (__uint128_t)mid * (mid + 1) / 2 ) <= n )
            lo = mid;
        else
            hi = mid - 1;
    }
    idx_t d = lo;                  /* diagonal index */
    __uint128_t tri = (__uint128_t)d * (d + 1) / 2;
    idx_t offset = n - (idx_t)tri; /* position on diagonal, 0 <= offset <= d */
    idx_t i = d - offset;          /* row index (n)   */
    idx_t j = offset;              /* column index (k) */

    /* Compute T(i,j) = j * ((1 + 2*i)*j - 1) / 2 using 128‑bit intermediates. */
    __uint128_t two_i   = (__uint128_t)2 * i;
    __uint128_t inner   = ( (__uint128_t)1 + two_i ) * ((__uint128_t)j) - 1;
    __uint128_t value128 = ((__uint128_t)j) * inner / 2;
    return (seq_t)value128;
}
