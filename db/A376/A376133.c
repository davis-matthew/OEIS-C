/*
 * A376133  Triangle T read by rows: T(n, 1) = (2*n*n - 4*n + 7 + (-1)^n) / 4
 *          and T(n, k) = T(n, k-1) + (-1)^k * 2 * (n+1-k) for k >= 2.
 * https://oeis.org/A376133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) enumerates the triangle T(n,k) row‑major. T(n,1) = (2*n*n -
 *            4*n + 7 + (-1)^n) / 4 T(n,k) = T(n,k-1) + (-1)^k * 2 * (n+1-k)
 *            for k >= 2 The sequence a(n) is defined for n >= 1, where
 *            a(1)=T(1,1)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* out of bounds, return 0 */
        return (seq_t)0;

    /* Find the row r such that r(r+1)/2 >= n and (r-1)r/2 < n. */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Index within the row, 1‑based. */
    idx_t k = n - r * (r - 1) / 2;

    /* Compute T(r,1) : base value of the row. Use signed arithmetic for safety. */
    long long rn = (long long)r;           /* promote to signed 64‑bit */
    long long base = 2 * rn * rn - 4 * rn + 7;
    base += ( (rn & 1) == 0 ) ? 1 : -1;    /* (-1)^r */

    base /= 4;  /* division is exact */
    long long cur = base;

    /* Build up to column k. */
    long long rk = rn + 1;   /* n+1 used in recurrence */
    for (idx_t j = 2; j <= k; ++j) {
        long long term = 2 * (rk - (long long)j);
        if ( (j & 1) == 0 )
            cur += term;     /* (+1) * term */
        else
            cur -= term;     /* (-1) * term */
    }

    return (seq_t)cur;
}
