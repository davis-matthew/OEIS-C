/*
 * A251139  Number of distinct prime factors of A098548(n).
 * https://oeis.org/A251139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = omega(A098548(n)); A098548: a(1)=1, a(2)=2, a(3)=3, a(n)
 *            = smallest m > a(n-1) with gcd(m,a(n-2))>1 and gcd(m,a(n-1))=1
 *            (computed by search)
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 9664 (2026-11-07)
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
    seq_t cur   = (seq_t)0;

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

static seq_t A001221_omega(seq_t m)
{
    seq_t count = 0;
    seq_t temp = m;
    if (temp <= 1)
        return 0;
    if (temp % 2 == 0) {
        count++;
        while (temp % 2 == 0)
            temp /= 2;
    }
    for (seq_t d = 3; d <= temp / d; d += 2) {
        if (temp % d == 0) {
            count++;
            while (temp % d == 0)
                temp /= d;
        }
    }
    if (temp > 1)
        count++;
    return count;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    return A001221_omega(A098548_a(n));
}
