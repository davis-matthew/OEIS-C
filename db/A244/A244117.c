/*
 * A244117  Triangle read by rows: terms of a binomial decomposition of 1 as
 *          Sum(k=0..n)T(n,k).
 * https://oeis.org/A244117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r, k) with r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2. T(n,k) = (1-k)^(k-1) * k^(n-k) * binomial(n,k) for
 *            k>0, T(n,0) = (n==0 ? 1 : 0)
 * Range:     i8 n <= 17, i16 n <= 31, i32 n <= 62, i64 n <= 147,
 *            i128 n <= 394, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"
#include "../utils/binomial.h"

static inline uint64_t sqrt_uint128(unsigned __int128 x)
{
    uint64_t lo = 0, hi = (uint64_t)-1;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Find row r */
    unsigned __int128 val = (unsigned __int128)n * 8 + 1;
    uint64_t s = sqrt_uint128(val);
    uint64_t r = (s - 1) / 2;

    /* Column k within row r */
    unsigned __int128 rk = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)rk;

    /* Handle k = 0 case */
    if (k == 0) {
        return (r == 0) ? 1 : 0;
    }

    /* Compute (1-k)^(k-1) */
    seq_t term1;
    if (k == 1) {
        term1 = 1;
    } else {
        term1 = big_pow((seq_t)(1 - (int64_t)k), (seq_t)(k - 1));
    }

    /* Compute k^(r-k) */
    seq_t term2 = big_pow((seq_t)k, (seq_t)(r - k));

    /* Binomial coefficient */
    seq_t binom = binomial((idx_t)r, k);

    /* Final product */
    return term1 * term2 * binom;
}
