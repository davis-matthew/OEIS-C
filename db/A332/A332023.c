/*
 * A332023  T(n, k) = binomial(n+2, 3) + binomial(k+1, 2) + binomial(k, 1).
 *          Triangle read by rows, T(n, k) for 0 <= k <= n.
 * https://oeis.org/A332023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (1/6)*(3*k^2 + 9*k + n*(n+1)*(n+2)) where (n,k) are
 *            obtained from the antidiagonal index: let r be the unique
 *            integer with r*(r+1)/2 <= n < (r+1)*(r+2)/2. Then k = n -
 *            r*(r+1)/2
 * Range:     u8 n <= 17, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* integer square root: largest s with s*s <= x */
static idx_t isqrt_u64(idx_t x)
{
    idx_t s = (idx_t)sqrtl((long double)x);
    /* correct possible round‑off errors */
    while ((unsigned __int128)(s + 1) * (s + 1) <= x) ++s;
    while ((unsigned __int128)s * s > x) --s;
    return s;
}

seq_t a(idx_t n)
{
    /* determine the row r */
    idx_t s = isqrt_u64(8 * n + 1);
    idx_t r = (s - 1) / 2;

    /* starting index of row r */
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;

    /* compute T(r,k) using integer arithmetic on seq_t */
    seq_t rn = (seq_t)r;
    seq_t kn = (seq_t)k;
    seq_t term = (seq_t)3 * kn * kn + (seq_t)9 * kn
                + rn * (rn + 1) * (rn + 2);
    term /= (seq_t)6;
    return term;
}
