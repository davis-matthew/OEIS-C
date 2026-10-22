/*
 * A354599  Maximal GCD of nine positive integers with sum n.
 * https://oeis.org/A354599
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the largest divisor d of n such that d ≤ n/9. *
 *            Equivalently, a(n) = max{ d | d divides n, d ≤ n/9 }
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root using Newton's method.  Returns floor(sqrt(n)). */
static idx_t isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t x = n;
    while (1) {
        idx_t y = (x + n / x) >> 1;
        if (y >= x) break;
        x = y;
    }
    return x;
}

seq_t a(idx_t n)
{
    /* n >= 9 by definition */
    idx_t max_div = n / 9;          /* upper bound for the gcd */
    idx_t best = 1;                 /* at least 1 is always a divisor */

    idx_t limit = isqrt_u64(n);     /* only need to check up to sqrt(n) */

    for (idx_t d = 1; d <= limit; ++d) {
        if (n % d == 0) {
            if (d <= max_div && d > best) best = d;
            idx_t q = n / d;       /* complementary divisor */
            if (q != d && q <= max_div && q > best) best = q;
        }
    }
    return (seq_t)best;
}
