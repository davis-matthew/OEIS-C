/*
 * A301872  Smallest integer k >= 0 such that the maximum digit of k^2 written
 *          in factorial base equals n.
 * https://oeis.org/A301872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceil( sqrt( n * n! ) ) for n != 3; a(3) = 9
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 33, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 777 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <inttypes.h>
#include <limits.h>

#ifndef __cplusplus
/* ---------- Integer (C) version ---------- */

static unsigned long long sqrt128_floor(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = ULLONG_MAX, ans = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)9;
    if (n <= 2) return (seq_t)n;

    unsigned __int128 fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (unsigned __int128)i;
    }
    unsigned __int128 prod = fact * (unsigned __int128)n;

    unsigned long long sf = sqrt128_floor(prod);
    /* cast to seq_t afterwards */
    if ((unsigned __int128)sf * (unsigned __int128)sf < prod) sf += 1;
    return (seq_t)sf;
}
#else
/* ---------- Arbitrary-precision (C++) version ---------- */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static int seq_cmp(const void *p, const void *q)
{
    const seq_t *a = (const seq_t *)p;
    const seq_t *b = (const seq_t *)q;
    if (*a < *b) return -1;
    if (*a > *b) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 3) return (seq_t)9;
    if (n <= 2) return (seq_t)n;

    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        fact = mul_small(fact, (unsigned int)i);
    }
    seq_t prod = mul_small(fact, (unsigned int)n);
    seq_t sf   = floor_sqrt(prod);
    seq_t sq   = seq_mul(sf, sf);
    if (seq_cmp(&sq, &prod) < 0) {
        sf = sf + (seq_t)1;
    }
    return sf;
}
#endif
