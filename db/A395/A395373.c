/*
 * A395373  Triangle read by rows: T(n, k) is the number of ordered pairs
 *          (x,y) with 1 <= x,y <= n such that x*y = k, for 1 <= k <= n^2.
 * https://oeis.org/A395373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r, k) where r is the smallest integer with
 *            r(r+1)(2r+1)/6 >= n k = n - (r-1)r(2r-1)/6 (1 <= k <= r²) T(r,
 *            k) = #{d | k : d <= r and (k/d) <= r} (ordered factor pairs)
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_isqrt_u64.h"

static inline idx_t cum_terms(idx_t r)
{
    /* r(r+1)(2r+1) fits in 64 bits for all practical r (≤ 10⁴), but
       we use 128‑bit intermediate to avoid overflow. */
    __uint128_t v = (__uint128_t)r * (r + 1) * (2 * r + 1);
    v /= 6;
    return (idx_t)v;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* defensive, a(0) is unused */

    /* Find the row r such that cum_terms(r-1) < n <= cum_terms(r) */
    idx_t r = 1;
    while (cum_terms(r) < n)
        ++r;

    idx_t prev = cum_terms(r - 1);   /* total terms before this row        */
    idx_t k    = n - prev;           /* 1‑based position inside the row    */

    seq_t count = 0;
    idx_t limit = floor_isqrt_u64(k); /* ⌊√k⌋ */
    for (idx_t d = 1; d <= limit; ++d)
    {
        if (k % d == 0)
        {
            idx_t e = k / d;
            if (d <= r && e <= r)
                count += (d == e) ? 1 : 2;   /* ordered pairs */
        }
    }
    return count;
}
