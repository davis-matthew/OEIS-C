/*
 * A251104  Largest prime factor of A098548(n).
 * https://oeis.org/A251104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A006530(A098548(n)), where A098548(1..3) = 1,2,3 and
 *            A098548(n) is the smallest number > A098548(n-1) sharing a
 *            factor with A098548(n-2) and none with A098548(n-1); computed by
 *            direct search
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 9446 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static seq_t A098548_a(idx_t n)
{
    if (n <= 3)
        return (seq_t)n;

    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)3;
    seq_t cur = (seq_t)0;

    for (idx_t k = 4; k <= n; ++k) {
        seq_t candidate = prev1 + (seq_t)1;
        for (;;) {
            if (gcd(candidate, prev2) != (seq_t)1 &&
                gcd(candidate, prev1) == (seq_t)1) {
                cur = candidate;
                break;
            }
            ++candidate;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

static seq_t A006530_gpf(seq_t m)
{
    if (m <= (seq_t)1)
        return (seq_t)1;
    seq_t gpf = (seq_t)1;
    while (m % 2 == 0) {
        gpf = (seq_t)2;
        m = m / 2;
    }
    for (seq_t i = 3; i * i <= m; i += 2) {
        while (m % i == 0) {
            gpf = i;
            m = m / i;
        }
    }
    if (m > 1)
        gpf = m;
    return gpf;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    return A006530_gpf(A098548_a(n));
}
