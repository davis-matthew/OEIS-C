/*
 * A365876  a(n) is the number of quadratic equations u*x^2 + v*x + w = 0 with
 *          distinct solution sets L != {}, integer coefficients u, v, w and
 *          GCD(u, v, w) = 1, where n = abs(u) + abs(v) + abs(w) and the sum
 *          of the solutions equals the product of the solutions.
 * https://oeis.org/A365876
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count solutions (u>0, v∈ℤ) with n = |u| + 2|v|, gcd(u,|v|)=1,
 *            and discriminant v²+4uv ≥ 0
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t gcd_u64(idx_t a, idx_t b) {
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    seq_t cnt = 0;
    for (idx_t u = 1; u <= n; ++u) {
        idx_t m = n - u;
        if (m & 1u) continue;          /* |v| must be an integer */
        idx_t absv = m >> 1;           /* |v| = (n - u)/2 */
        if (gcd_u64(u, absv) != 1u) continue;
        cnt++;                         /* v = +|v| always valid */
        if (absv > 0 && absv >= (u << 2)) cnt++; /* v = -|v| if allowed */
    }
    return cnt;
}
