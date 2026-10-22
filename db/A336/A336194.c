/*
 * A336194  Table read by antidiagonals upwards: T(n,k) = (n - 1)*k^3 - 1,
 *          with n > 1 and k > 0.
 * https://oeis.org/A336194
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Antidiagonal table read of T(n,k) = (n‑1)*k³ – 1; the sequence starts at index 2.
   For a given index idx >= 2 we first find the diagonal sum s such that
   C(s) = (s‑2)(s‑1)/2 >= idx‑1, then compute n=s‑p and k=p where
   p = (idx‑1) – C(s‑1).  Finally return (n‑1)*k³ – 1. */

#include <stdint.h>
#include <stdbool.h>

/* integer floor of sqrt for 128‑bit values */
static inline uint64_t floor_sqrt_u128(__uint128_t x) {
    __uint128_t lo = 0, hi = (__uint128_t)1 << 64;   /* hi is exclusive */
    while (lo + 1 < hi) {
        __uint128_t mid = (lo + hi) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid;
    }
    return (uint64_t)lo;
}

/* Compute a(idx) for OEIS A336194 */
seq_t a(idx_t n) {
    if (n < 2) return 0;                 /* behavior below offset irrelevant */

    /* 1‑based position in the antidiagonal ordering */
    idx_t m = n - 1;

    /* find minimal t >= 1 with t(t+1)/2 >= m */
    __uint128_t val = (__uint128_t)8 * m + 1;
    uint64_t s1 = floor_sqrt_u128(val);
    uint64_t t = (s1 - 1) / 2;            /* lower bound for t */
    if ((__uint128_t)t * (t + 1) / 2 < m) t++;   /* ensure t is minimal */

    uint64_t s = t + 2;                   /* diagonal sum n + k */

    /* cumulative count up to s‑1 */
    uint64_t sm1 = s - 1;
    __uint128_t c_sm1 = (__uint128_t)(sm1 - 2) * (sm1 - 1) / 2;
    uint64_t p = m - (uint64_t)c_sm1;     /* 1‑based position inside diagonal */

    uint64_t nn = s - p;                  /* n = s – p */
    uint64_t k  = p;                      /* k = p */

    /* compute (n‑1)*k³ – 1 */
    __uint128_t k3 = (__uint128_t)k * k * k;
    __uint128_t result = (__uint128_t)(nn - 1) * k3 - 1;

    return (seq_t)result;
}
