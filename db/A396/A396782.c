/*
 * A396782  Index of 2^n - 1 in A007665.
 * https://oeis.org/A396782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -1 + (n-k)^3/6 + 5*(n-k)/6 + 2^k where k is the unique
 *            integer with k + 2^(k/2) - 1/2 < n < (k+1)+2^((k+1)/2) - 1/2
 *            (i.e. f(k) < n < f(k+1) with f(k) = k + 2^(k/2) - 1/2 ) The
 *            function works for all n ≥ 1
 * Range:     u8 n <= 16, u16 n <= 83, u32 n <= 1859, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    /* simple integer square‑root for small n (≤ 2^63).  Works in any build. */
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= n) ++r;
    return r;
}

/* pre‑computed floor values of f(k) for k = 0 … 63 */
static idx_t f_floor[64];
static int f_floor_initialized = 0;

static void init_f_floor(void)
{
    if (f_floor_initialized) return;
    for (idx_t k = 0; k < 64; ++k) {
        if ((k & 1) == 0) {                /* even k = 2m */
            idx_t m = k >> 1;
            f_floor[k] = k + (1ULL << m) - 1; /* floor(2m + 2^m - 0.5) = 2m + 2^m - 1 */
        } else {                           /* odd k = 2m+1 */
            idx_t m = (k - 1) >> 1;
            idx_t X = 1ULL << m;           /* 2^m */
            /* t = floor( sqrt(2) * X ) = floor( sqrt(2*X^2) ) */
            idx_t t = isqrt_u64(2 * X * X);
            /* check if sqrt(2)*X >= t + 0.5 */
            /* equivalent to 8*X^2 >= (2*t+1)^2 */
            unsigned __int128 lhs = (unsigned __int128)8 * X * X;
            unsigned __int128 rhs = (unsigned __int128)(2 * t + 1) * (2 * t + 1);
            idx_t t_minus_half = (lhs >= rhs) ? t : t - 1;
            f_floor[k] = k + t_minus_half;
        }
    }
    f_floor_initialized = 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n=0, but safe */
    init_f_floor();

    /* find the largest k with f_floor[k] < n */
    idx_t k = 0;
    while (k + 1 < 64 && f_floor[k] < n) ++k;
    /* Now f_floor[k] >= n or k==63.  We need the preceding k where f(k) < n. */
    if (f_floor[k] >= n && k > 0) --k;
    /* If floor already < n, k stays. For n=1, k will be 0. */

    idx_t d = n - k;                /* d = n - k */

    seq_t d_seq = (seq_t)d;
    seq_t term = d_seq * (d_seq * d_seq + (seq_t)5) / (seq_t)6;
    seq_t pow2k = (seq_t)1 << k;     /* 2^k in seq_t arithmetic */

    return term + pow2k - (seq_t)1;
}
