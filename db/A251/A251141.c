/*
 * A251141  Total number of prime factors of A098548(n).
 * https://oeis.org/A251141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A098548(n)); A098548 computed by direct search:
 *            a(1..3)=1,2,3, a(n)=least m>a(n-1) with gcd(m,a(n-2))>1 and
 *            gcd(m,a(n-1))=1
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 102 terms through n = 7558 (2026-11-07)
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
        seq_t cand = prev1 + (seq_t)1;
        for (;;) {
            if (gcd(cand, prev2) != (seq_t)1 && gcd(cand, prev1) == (seq_t)1) {
                cur = cand;
                break;
            }
            ++cand;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

static seq_t A251141_bigomega(seq_t m)
{
    seq_t count = 0;
    while (m % 2 == 0) {
        count++;
        m /= 2;
    }
    for (seq_t i = 3; i <= m / i; i += 2) {
        while (m % i == 0) {
            count++;
            m /= i;
        }
    }
    if (m > 1)
        count++;
    return count;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    return A251141_bigomega(A098548_a(n));
}
