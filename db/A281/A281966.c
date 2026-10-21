/*
 * A281966  Number of sets of exactly n positive integers <= n+3 having a
 *          square element sum.
 * https://oeis.org/A281966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of subsets of size n drawn from {1,…,n+3} whose
 *            element sum is a perfect square. This is computed by counting
 *            the 3‑element omitted triples {x,y,z} (1≤x<y<z≤n+3) such that
 *            (n+3)(n+4)/2 – (x+y+z) is a square
 * Range:     u8 n <= 44, u16 n <= 721, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t isqrt_u64(uint64_t n) {
    uint64_t lo = 0, hi = n, res = 0;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq <= n) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

static inline uint64_t ceil_isqrt_u64(uint64_t n) {
    uint64_t r = isqrt_u64(n);
    return ( (__uint128_t)r * r == n ) ? r : r + 1;
}

seq_t a(idx_t n) {
    /* Basic parameters */
    const uint64_t N = (uint64_t)(n) + 3;                /* upper bound of numbers */
    const uint64_t T = N * (N + 1) / 2;                  /* sum of 1..N */

    /* Range of k such that S = T - k^2 lies between 6 and 3N-3 */
    const uint64_t min_val = T - 3 * N + 3;              /* T - (3N-3) */
    const uint64_t k_min = ceil_isqrt_u64(min_val);      /* ceil(sqrt(T-3N+3)) */
    const uint64_t k_max = isqrt_u64(T - 6);             /* floor(sqrt(T-6)) */

    uint64_t total = 0;

    for (uint64_t k = k_min; k <= k_max; ++k) {
        const uint64_t S = T - k * k;                    /* sum omitted = x+y+z */

        /* Count triples x<y<z with sum S */
        for (uint64_t x = 1; x + 2 <= N; ++x) {
            const long long t = (long long)S - (long long)x;
            if (t <= 0) continue;                       /* impossible if t <= 0 */

            long long y_low  = (long long)x + 1;
            long long th_N   = t - (long long)N;
            if (th_N > y_low) y_low = th_N;

            long long y_high = (t - 1) / 2;              /* floor((t-1)/2) */

            if (y_high >= y_low) {
                total += (uint64_t)(y_high - y_low + 1);
            }
        }
    }

    return (seq_t)total;
}
