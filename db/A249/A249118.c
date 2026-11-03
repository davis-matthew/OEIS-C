/*
 * A249118  Position of 32n^6 in the ordered union of {h^6, h >=1} and
 *          {32*k^6, k >=1}.
 * https://oeis.org/A249118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (32)^(1/6) * n ) + n The position of 32*n^6 in the
 *            ordered union of {h^6 : h≥1} and {32*k^6 : k≥1}. This uses only
 *            integer arithmetic
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 59 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*  Find the largest integer h such that h^6 ≤ 32 * n^6.
    The computation is performed in a 128‑bit intermediate type
    to avoid overflow for n up to 10 000.  The function returns
    the value as an idx_t (uint64_t). */
static idx_t _floor_sixth_root(idx_t n)
{
    /* Compute 32 * n^6 in 128‑bit */
    __uint128_t n128 = n;
    __uint128_t n6   = 1;
    for (int i = 0; i < 6; ++i) n6 *= n128;
    __uint128_t m = n6 * 32;

    /* Search range: the root is at most ⌊2 * n⌋ + 10 for all n≥1. */
    idx_t low  = 0;
    idx_t high = n * 2 + 10;
    idx_t ans  = 0;

    while (low <= high)
    {
        idx_t mid = low + ((high - low) >> 1);
        __uint128_t mid128 = mid;
        __uint128_t mid6   = 1;
        for (int i = 0; i < 6; ++i) mid6 *= mid128;
        if (mid6 <= m)
        {
            ans = mid;
            low = mid + 1;
        }
        else
        {
            if (mid == 0) break;  /* avoid underflow */
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* value below the offset is irrelevant */
    idx_t hmax = _floor_sixth_root(n);   /* floor((32)^(1/6) * n) */
    seq_t n_t  = (seq_t)n;
    return (seq_t)hmax + n_t;      /* position of 32*n^6 in the union */
}
