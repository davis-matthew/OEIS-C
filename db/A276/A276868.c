/*
 * A276868  First differences of the Beatty sequence A276855 for 3 + phi,
 *          where phi = golden ratio = (1 + sqrt(5))/2.
 * https://oeis.org/A276868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*r) - floor((n-1)*r) , r = 3 + φ = (7+√5)/2
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline uint64_t isqrt_u128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t) -1;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 mid2 = (unsigned __int128)mid * mid;
        if (mid2 <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* compute floor(n*φ) where φ = √5 */
static inline seq_t floor_n_phi(idx_t n)
{
    unsigned __int128 n128 = n;
    unsigned __int128 val = (unsigned __int128)5 * n128 * n128; /* 5*n^2 */
    uint64_t s = isqrt_u128(val);                              /* floor(√(5*n^2)) == floor(n*√5) */
    return (seq_t)s;
}

/* compute floor(n*r) with r = 3 + φ = (7+√5)/2 */
static inline seq_t floor_n_r(idx_t n)
{
    seq_t phi_floor = floor_n_phi(n);
    return (((seq_t)7 * n) + phi_floor) / 2;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1, but guard for n=0 */
    seq_t curr = floor_n_r(n);
    seq_t prev = floor_n_r(n - 1);
    return curr - prev;
}
