/*
 * A386217  Values of v in the (1,3)-quartals (m,u,v,w) having m=3; i.e.,
 *          values of v for solutions to 3 + u^3 = v + w^3, in positive
 *          integers, with m<v; see Comments.
 * https://oeis.org/A386217
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3 + 3u^2k - 3uk^2 + k^3 where u is the smallest integer
 *            with u(u-1)/2 ≥ n, and k = n - (u-1)(u-2)/2. This enumerates
 *            v-values for (1,3)-quartals with m = 3
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static idx_t isqrt_u64(idx_t x) {
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n) {
    /* find u: smallest integer with u(u-1)/2 >= n */
    idx_t D = n * 8 + 1;                    /* 1 + 8n */
    idx_t sqrtD = isqrt_u64(D);
    idx_t u = (sqrtD + 1) / 2;              /* floor of root */
    if (u * (u - 1) / 2 < n)                /* adjust if necessary */
        u++;

    /* k = n - (u-1)(u-2)/2  (1 <= k <= u-1) */
    idx_t T_prev = (u - 1) * (u - 2) / 2;
    idx_t k = n - T_prev;

    /* compute the v-value using wider integer arithmetic to avoid overflow */
    unsigned long long u_ll = u;
    unsigned long long k_ll = k;

    unsigned long long val =
        3ULL
        + 3ULL * u_ll * u_ll * k_ll
        - 3ULL * u_ll * k_ll * k_ll
        + k_ll * k_ll * k_ll;

    return (seq_t)val;
}
