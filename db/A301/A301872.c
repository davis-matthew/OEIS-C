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
 * Helpers:   floor_sqrt, mul_small, seq_cmp, seq_mul
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

#include "../utils/mul_small.h"
#include "../utils/floor_sqrt.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_cmp.h"

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
