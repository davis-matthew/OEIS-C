/*
 * A290542  a(n) is the least integer k in the interval [2, sqrt(n)] such that
 *          k^n == k (mod n), or 0 if no such integer exists.
 * https://oeis.org/A290542
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   least k in [2, ⌊√n⌋] with kⁿ ≡ k (mod n), or 0 if none
 * Range:     u8 n <= 1170, u16 n <= 9099, u32 n <= 9099, u64 n <= 9099,
 *            u128 n <= 9099, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mod_pow.h"

/* Integer floor‑square‑root for idx_t (uint64_t). */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    if (n == 0 || n == 1)
        return n;
    idx_t low = 0, high = n, best = 0;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        /* use 128‑bit to avoid overflow */
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            best = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;             /* offset is 4; behaviour below is irrelevant */

    idx_t limit = floor_isqrt_u64(n);
    seq_t mod = (seq_t)n;

    for (idx_t k = 2; k <= limit; ++k) {
        if (mod_pow((seq_t)k, n, mod) == (seq_t)k)
            return (seq_t)k;
    }
    return 0;
}
