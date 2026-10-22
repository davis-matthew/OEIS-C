/*
 * A336225  Table read by antidiagonals: T(n, k) = digitsum(n*k) with n >= 0
 *          and k >= 0.
 * https://oeis.org/A336225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl, easy
 * Formula:   a(n) = digit sum of i*j, where (i,j) are the antidiagonal
 *            coordinates of n: s = floor((sqrt(8*n+1)-1)/2), offset =
 *            s*(s+1)/2, k = n - offset, i = k, j = s - k. The product can be
 *            large; the digit sum is computed in 128‑bit arithmetic
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal number s such that s*(s+1)/2 <= n < (s+1)*(s+2)/2. */
    idx_t s = 0;
    while (((__uint128_t)(s + 1) * (s + 2)) / 2 <= n)
        ++s;

    /* Compute offset of the first element of this diagonal. */
    idx_t offset = ((__uint128_t)s * (s + 1)) / 2;

    /* Co‑ordinates (i,j) on the diagonal. */
    idx_t k = n - offset;   /* 0 <= k <= s */
    idx_t i = k;
    idx_t j = s - k;

    /* Compute product and its digit sum using 128‑bit arithmetic. */
    __uint128_t prod = (__uint128_t)i * j;
    seq_t sum = 0;
    while (prod > 0)
    {
        sum += (seq_t)(prod % 10);  /* cast to seq_t (<10) */
        prod /= 10;
    }

    return sum;
}
