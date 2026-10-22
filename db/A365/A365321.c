/*
 * A365321  Number of pairs of distinct positive integers <= n that cannot be
 *          linearly combined with positive coefficients to obtain n.
 * https://oeis.org/A365321
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (x,y) with 1 <= x < y <= n such that * n
 *            cannot be expressed as a*x + b*y with a,b >= 1
 * Range:     u8 n <= 26, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   ceil_div, extended_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ceil_div.h"
#include "../utils/extended_gcd.h"

static long long floor_div(long long a, long long b) /* b > 0 */
{
    if (a >= 0) return a / b;
    return -(( -a + b - 1) / b);
}

seq_t a(idx_t n)
{
    if (n < 2) return 0ULL;           /* a(0)=0, a(1)=0 */

    seq_t result = 0ULL;
    idx_t i, j;

    for (i = 1; i <= n; ++i) {
        for (j = i + 1; j <= n; ++j) {
            /* If the smallest positive linear combination is already > n,
               the pair cannot represent n. */
            if (i + j > n) {
                result += 1;
                continue;
            }

            /* Solve a*i + b*j = n with a,b >= 1 */
            long long x = (long long)i;
            long long y = (long long)j;
            long long d = 0;
            long long u, v;

            d = extended_gcd(x, y, &u, &v);
            if (n % d != 0) {
                /* No integer solution at all */
                result += 1;
                continue;
            }

            long long mul = (long long)(n / d);
            long long a0 = u * mul;
            long long b0 = v * mul;
            long long x_div = x / d;  /* > 0 */
            long long y_div = y / d;  /* > 0 */

            long long t_low  = ceil_div(1 - a0, y_div);
            long long t_high = floor_div(b0 - 1, x_div);

            if (t_low > t_high) {
                /* No non‑negative t gives a,b >= 1 */
                result += 1;
            }
        }
    }

    return result;
}
