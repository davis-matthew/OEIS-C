/*
 * A251090  Squarefree kernel of A098548(n).
 * https://oeis.org/A251090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A007947(A098548(n)); A098548 computed by direct search:
 *            a(1)=1, a(2)=2, a(3)=3, a(n) = least m > a(n-1) with
 *            gcd(m,a(n-2))>1 and gcd(m,a(n-1))=1
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 78 terms through n = 9366 (2026-11-07)
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

static seq_t A007947_a(seq_t m)
{
    seq_t result = 1;
    if (m % 2 == 0) {
        result *= 2;
        while (m % 2 == 0)
            m /= 2;
    }
    for (seq_t p = 3; p <= m / p; p += 2) {
        if (m % p == 0) {
            result *= p;
            while (m % p == 0)
                m /= p;
        }
    }
    if (m > 1)
        result *= m;
    return result;
}

seq_t a(idx_t n)
{
    return A007947_a(A098548_a(n));
}
