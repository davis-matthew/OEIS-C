/*
 * A349454  Number T(n,k) of endofunctions on [n] with exactly k fixed points,
 *          all of which are isolated; triangle T(n,k), n>=0, 0<=k<=n, read by
 *          rows.
 * https://oeis.org/A349454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(r, k) * (r - k - 1)^(r - k) where r =
 *            floor((sqrt(8*n+1)-1)/2) and k = n - r(r+1)/2
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 152,
 *            u128 n <= 405, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Integer square‑root for idx_t (non‑recursive, binary search). */
static idx_t int_sqrt_u64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = x;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t sq  = mid * mid;
        if (sq == x)           return mid;
        else if (sq <  x)      lo = mid + 1;
        else                   hi = mid - 1;
    }
    return hi;   /* hi is floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Row index r */
    idx_t tmp = 8 * n + 1;
    idx_t sq = int_sqrt_u64(tmp);
    idx_t r  = (sq - 1) >> 1;

    /* Column index k within that row */
    idx_t sum_prev = r * (r + 1) / 2;   /* number of elements before row r */
    idx_t k = n - sum_prev;             /* 0 <= k <= r */

    /* Binomial coefficient */
    seq_t bin = binomial(r, k);

    /* If k == r, the power factor is (-1)^0 = 1 */
    if (k == r) return bin;   /* binomial(r,r) == 1 */

    /* Power part: (r - k - 1)^(r - k) */
    seq_t base = (seq_t)(r - k - 1);
    seq_t exp  = (seq_t)(r - k);
    seq_t powv = big_pow(base, exp);

    return bin * powv;
}
