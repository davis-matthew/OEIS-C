/*
 * A380967  a(n) is the largest number in row n of A380920.
 * https://oeis.org/A380967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   T(n,k) by iterating b(n,k,i+1) = i*b/floor(i*b) from b(n,k,1) =
 *            n/k, skipping the steps where b does not change: with b = c/d
 *            (reduced) and e = c-d > 0, b stays the same while i*e < d, so
 *            the next change is at i' = max(i, ceil(d/e)). Write q =
 *            floor(i'*e/d) and r = i'*e mod d. If r = 0 then T = i...
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 78, u64 n <= 162,
 *            u128 n <= 315, big any n
 * Validated: standard, pass, 65 terms through n = 8595 (2026-11-10)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t A380967_T(idx_t n, idx_t k)
{
    seq_t c = (seq_t)n;
    seq_t d = (seq_t)k;
    seq_t g = gcd(c, d);
    c /= g;
    d /= g;
    if (c == d) return (seq_t)1;
    seq_t i = 1;
    for (;;) {
        seq_t e = c - d;                 /* > 0 */
        seq_t ip = (d + e - 1) / e;      /* ceil(d/e) */
        if (ip < i) ip = i;
        seq_t t = ip * e;
        seq_t q = t / d;
        seq_t r = t % d;
        if (r == 0) return ip + 1;
        seq_t nn = ip * c;
        seq_t nd = d * (ip + q);
        seq_t h = gcd(nn, nd);
        c = nn / h;
        d = nd / h;
        i = ip + 1;
    }
}

seq_t a(idx_t n)
{
    seq_t best = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t t = A380967_T(n, k);
        if (t > best) best = t;
    }
    return best;
}
