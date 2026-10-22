/*
 * A386215  Values of v in the (1,3)-quartals (m,u,v,w) having m=2; i.e.,
 *          values of v for solutions to m + u^3 = v + w^3, in positive
 *          integers, with m<v, sorted by u, then v.
 * https://oeis.org/A386215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 + u^3 - (u*(u-1)/2 + 1 - n)^3, where u = floor((3 +
 *            sqrt(8*n-7))/2)
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* integer square root of a 64‑bit number using binary search */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = (UINT64_C(1) << 32);
    while (lo < hi) {
        uint64_t mid = (lo + hi) >> 1;
        if ((unsigned __int128)mid * mid <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined for n<1, return 0 */
        return 0;

    /* 8*n - 7 with 128‑bit arithmetic to avoid overflow */
    unsigned __int128 tmp = (unsigned __int128)8 * n - 7;
    uint64_t val64 = (uint64_t)tmp;                /* <= 2^64-1 for required n */

    /* u = floor((3 + sqrt(val64))/2) */
    uint64_t s = isqrt_u64(val64);                /* floor(sqrt(8*n-7)) */
    uint64_t u = (3 + s) >> 1;                    /* divide by 2 */

    /* triangular number part: u*(u-1)/2 */
    uint64_t tri = u * (u - 1) / 2;

    /* t = tri + 1 - n (may be negative) */
    int64_t t = (int64_t)tri + 1 - (int64_t)n;
    uint64_t t_abs = (t >= 0) ? (uint64_t)t : (uint64_t)(-t);

    /* cubes using 128‑bit to stay safe */
    unsigned __int128 t_cub = (unsigned __int128)t_abs *
                              (unsigned __int128)t_abs *
                              (unsigned __int128)t_abs;
    unsigned __int128 u_cub = (unsigned __int128)u *
                              (unsigned __int128)u *
                              (unsigned __int128)u;

    unsigned __int128 res;
    if (t >= 0)
        res = (unsigned __int128)2 + u_cub - t_cub;
    else
        res = (unsigned __int128)2 + u_cub + t_cub;

    return (seq_t)res;
}
