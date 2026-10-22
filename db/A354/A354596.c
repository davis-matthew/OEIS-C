/*
 * A354596  Array T(n,k) = k^2 + (2n-4)*floor(k/2)^2, n >= 0, k >= 0, read by
 *          descending antidiagonals.
 * https://oeis.org/A354596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is defined by reading the array T(n,k) for n>=0,k>=0, where
 *            T(n,k) = k*k + (2*n-4)*floor(k/2)^2, along descending
 *            antidiagonals (constant n+k). For an index n write n = s(s+1)/2
 *            + r , 0 <= r <= s then a(n) = (s-r)^2 + (2*r-4)*floor((s-r)/2)^2
 *            . Complexity: O(1) per call. The routine works fo...
 * Range:     u8 n <= 139, u16 n <= 4687, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for unsigned 128‑bit numbers. */
static idx_t isqrt_u128(__uint128_t x)
{
    idx_t low = 0;
    idx_t high = (idx_t)-1;   /* maximum 64‑bit value */
    idx_t ans  = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t prod = (__uint128_t)mid * (__uint128_t)mid;
        if (prod <= x) {
            ans = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return ans;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* find diagonal index s such that (s+1)(s+2)/2 > n */
    __uint128_t t = (__uint128_t)8 * n + 1;         /* 8*n + 1 */
    idx_t sqrt_val = isqrt_u128(t);                /* floor(sqrt(8*n+1)) */
    idx_t s = (sqrt_val - 1) >> 1;                 /* (sqrt_val-1)/2 */

    /* base = number of terms up to diagonal s-1 */
    __uint128_t base128 = (__uint128_t)s * (__uint128_t)(s + 1) / 2;
    idx_t base = (idx_t)base128;

    idx_t r = n - base;          /* offset within diagonal s  (row index) */
    idx_t k = s - r;             /* column index */

    /* compute T(r,k) = k^2 + (2*r-4)*floor(k/2)^2 */
    idx_t hk = k >> 1;                     /* floor(k/2) */
    __int128 coeff = (__int128)2 * (__int128)r - 4;   /* can be negative */
    __int128 val128 = (__int128)k * (__int128)k
                      + coeff * (__int128)hk * (__int128)hk;

    return (seq_t)val128;
}
