/*
 * A377802  Triangle read by rows: T(n, k) = (2 * (n+1)^2 + 7 - (-1)^n) / 8 -
 *          k.
 * https://oeis.org/A377802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = row‑major read of triangle T(n,k) = (2*(n+1)^2 + 7 -
 *            (-1)^n)/8 - k, where k = n - T_{n-1}, T_n = n(n+1)/2
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static unsigned long long
isqrt128(unsigned __int128 x)
{
    unsigned long long lo = 0, hi = (unsigned long long)-1, ans = 0;
    while (lo <= hi) {
        unsigned long long mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x) {
            ans = mid;
            lo  = mid + 1;
        } else {
            if (mid == 0) break;
            hi  = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t N)
{
    /* 1‑based index: adjust only if N==0 (undefined in OEIS) */
    if (N == 0) return (seq_t)0;

    /* Find row n such that T_{n-1} < N <= T_n   (T_n = n(n+1)/2) */
    idx_t n = (idx_t)((isqrt128((unsigned __int128)8 * N + 1) - 1) / 2);
    if ((unsigned __int128)n * (n + 1) / 2 < N) n++;

    /* compute k = position within row n (1 ≤ k ≤ n) */
    idx_t prevTri = (unsigned __int128)(n - 1) * n / 2;
    idx_t k = N - prevTri;

    /* compute T(n,k) = (2*(n+1)^2 + 7 - (-1)^n)/8 - k */
    int sign = (n & 1) ? -1 : 1;          /* (-1)^n */
    unsigned __int128 s = (unsigned __int128)2 * (n + 1) * (n + 1)
                         + 7 - sign;      /* numerator of the fraction */
    seq_t term = (seq_t)(s / 8) - (seq_t)k; /* final value fits in seq_t */

    return term;
}
