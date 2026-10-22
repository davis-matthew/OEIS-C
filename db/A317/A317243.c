/*
 * A317243  a(n) is the number of open intervals (m, m+1) containing at least
 *          one fraction n/k, where m and k are integers between 1 and n.
 * https://oeis.org/A317243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (floor(sqrt(n)) + floor(sqrt(n-1)) + 2*floor(sqrt(n)-1/2)
 *            - tau(n))/2 where tau(n) is the number of positive divisors of n
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer floor square root for unsigned 64‑bit integers, avoiding any helper
   header that might not be available. */
static inline idx_t floor_sqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n)  lo = mid + 1;
        else         hi = mid - 1;
    }
    return hi;
}

/* number of positive divisors of a 64‑bit integer, implemented by trial division */
static inline seq_t divisor_count_u64(idx_t n) {
    seq_t tau = 1;
    idx_t m = n;
    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            int cnt = 0;
            while (m % p == 0) {
                m /= p;
                ++cnt;
            }
            tau *= (cnt + 1);
        }
    }
    if (m > 1) tau *= 2;
    return tau;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* protects against illegal use */

    idx_t sqrt_n  = floor_sqrt_u64(n);
    idx_t sqrt_n1 = (n > 0) ? floor_sqrt_u64(n - 1) : 0;

    /* floor(sqrt(n) - 1/2) */
    idx_t floor_half;
    if ((idx_t)sqrt_n * (idx_t)sqrt_n == n) {
        floor_half = sqrt_n - 1;
    } else {
        idx_t t   = sqrt_n;
        idx_t lc  = 4 * n;                     /* 4*n   */
        idx_t rc  = (2 * t + 1);
        rc = rc * rc;                          /* (2*t+1)^2 */
        floor_half = (lc >= rc) ? t : (t - 1);
    }

    seq_t tau = divisor_count_u64(n);

    seq_t val = (seq_t)sqrt_n + (seq_t)sqrt_n1 + 2 * (seq_t)floor_half - tau;
    return val / 2;
}
