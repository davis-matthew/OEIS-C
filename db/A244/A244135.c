/*
 * A244135  Triangle read by rows: terms T(n,k) of a binomial decomposition of
 *          n^n as Sum(k=0..n)T(n,k).
 * https://oeis.org/A244135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) enumerates the triangle entries T(r,k) in row‑major order,
 *            where T(r,k)=(-k)^(k-1)*(r+k)^(r-k)*binomial(r,k) with
 *            T(r,0)=0^r by convention (T(0,0)=1)
 * Range:     i8 n <= 10, i16 n <= 21, i32 n <= 47, i64 n <= 123,
 *            i128 n <= 333, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* integer square‑root of a 64‑bit unsigned integer, floor value */
static uint64_t isqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = 4294967295ULL, ans = 0;
    while (lo <= hi) {
        uint64_t mid = (lo + hi) >> 1;
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    uint64_t r = 0;
    if (n != 0) {
        uint64_t t = isqrt_u64(8 * n + 1);
        r = (t - 1) / 2;
        /* correct possible off‑by‑one due to floor(sqrt) */
        while (r * (r + 1) / 2 > n) --r;
        while ((r + 1) * (r + 2) / 2 <= n) ++r;
    }
    uint64_t col = n - r * (r + 1) / 2;   /* 0 ≤ col ≤ r */

    /* handle the 0‑th column directly */
    if (col == 0) return (r == 0) ? (seq_t)1 : (seq_t)0;

    /* T(r,col)=(-col)^(col-1)*(r+col)^(r-col)*binomial(r,col) */
    seq_t binom = binomial(r, col);

    /* compute |col|^(col-1) and apply sign */
    seq_t col_pow = big_pow((seq_t)col, (seq_t)(col - 1));
    if ((col - 1) & 1) col_pow = -col_pow;   /* negative if exponent odd */

    /* compute (r+col)^(r-col) */
    seq_t power_term = big_pow((seq_t)(r + col), (seq_t)(r - col));

    /* combine the components */
    seq_t result = col_pow;
    result *= power_term;
    result *= binom;

    return result;
}
