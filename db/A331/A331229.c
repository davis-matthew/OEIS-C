/*
 * A331229  a(n) = number of triangles with integer sides i <= j <= k with
 *          radius of circumcircle <= n.
 * https://oeis.org/A331229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   div_ceil128, div_floor128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = number of integer triangles i ≤ j ≤ k with
 * circumradius R(i,j,k) ≤ n
 *
 * R(i,j,k) = (i*j*k) /
 *            sqrt[(i+j+k)(-i+j+k)(i-j+k)(i+j-k)].
 *
 * The algorithm counts all pairs (i,j) and computes the range of
 * admissible k by solving a quadratic inequality derived from
 * R ≤ n.  Complexity ≈ O(n²);  n = 10000 -> about 2·10⁸ pair checks,
 * which easily fits the time limit in C.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/div_ceil128.h"
#include "../utils/div_floor128.h"

static inline uint64_t floor_sqrt_u64(uint64_t x) {
    uint64_t r = (uint64_t) sqrtl((long double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

static inline uint64_t ceil_sqrt_u64(uint64_t x) {
    uint64_t r = floor_sqrt_u64(x);
    return (r * r == x) ? r : r + 1;
}

/* floor sqrt of __int128 (non‑negative) */
static inline uint64_t isqrt128(__int128 v) {
    if (v <= 1) return (uint64_t)v;
    uint64_t high = (uint64_t) floor_sqrt_u64((uint64_t)v);
    uint64_t low  = high >> 1;
    while (low < high) {
        uint64_t mid = (low + high + 1) >> 1;
        __int128 m2 = (__int128)mid * mid;
        if (m2 <= v) low = mid;
        else high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    if ((idx_t)n == 0) return 0;           /* sequence offset 1 */
    uint64_t N = (uint64_t)n;
    uint64_t lim = 2 * N;                  /* maximum side length */
    __int128 NN = (__int128)N * (__int128)N; /* n^2 for later use */

    seq_t count = 0;

    for (uint64_t a = 1; a <= lim; ++a) {
        uint64_t a2 = a * (uint64_t)a;
        for (uint64_t b = a; b <= lim; ++b) {
            uint64_t b2 = b * (uint64_t)b;
            uint64_t d  = b - a;
            uint64_t d2 = d * (uint64_t)d;
            uint64_t S  = a + b;           /* a+b */
            uint64_t S2 = S * (uint64_t)S;
            uint64_t S1 = S - 1;           /* upper bound from triangle inequality */
            uint64_t S1_sq = S1 * (uint64_t)S1;

            /* Coefficients of quadratic: NN*t^2 - B*t + NN*S2*d2 <= 0   */
            __int128 B = NN * (__int128)S2 + NN * (__int128)d2 - (__int128)a2 * (__int128)b2;

            /* Discriminant: D = B^2 - 4*NN*NN*S2*d2 */
            __int128 fourNNNN = (__int128)4 * NN * NN;
            __int128 D = B * B - fourNNNN * (__int128)S2 * (__int128)d2;
            if (D < 0) continue;           /* no real roots --> no admissible k */

            uint64_t sqrtD = isqrt128(D);
            __int128 den = 2 * NN;

            uint64_t t_low = div_ceil128((__int128)B - (__int128)sqrtD, (uint64_t)den);
            uint64_t t_high = div_floor128((__int128)B + (__int128)sqrtD, (uint64_t)den);

            /* Intersect with c bounds: c >= b, c ≤ min(S-1, 2N) */
            uint64_t bound_low = b * (uint64_t)b;       /* b² */
            uint64_t bound_high = S1_sq;
            uint64_t hn2 = (2 * N) * (2 * N);
            if (hn2 < bound_high) bound_high = hn2;
            if (t_low < bound_low) t_low = bound_low;
            if (t_high > bound_high) t_high = bound_high;
            if (t_low > t_high) continue;

            /* Convert t bounds to k bounds */
            uint64_t k_min = ceil_sqrt_u64(t_low);
            if (k_min < b) k_min = b;
            uint64_t k_max = floor_sqrt_u64(t_high);
            if (k_max > S1) k_max = S1;            /* enforce triangle inequality */
            if (k_max > 2 * N) k_max = 2 * N;      /* enforce circumradius bound  */
            if (k_min > k_max) continue;

            count += (seq_t)(k_max - k_min + 1);
        }
    }

    return count;
}
