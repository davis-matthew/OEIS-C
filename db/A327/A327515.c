/*
 * A327515  Number of steps to reach a fixed point starting with n and
 *          repeatedly taking the quotient by the maximum divisor that is 1,
 *          2, or a nonprime number whose prime indices are pairwise coprime
 *          (A327512, A327514).
 * https://oeis.org/A327515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 if n is a fixed point of n -> A327514(n) =
 *            n/A327512(n), else a(n) = 1 + a(A327514(n)); computed by
 *            iterating the map (divisor search over prime-index subsets)
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9980 (2026-11-08)
 * Helpers:   gcd_u64, prime_pi_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/prime_pi_u64.h"
#include "../utils/gcd_u64.h"

static void A327515_dfs(const uint64_t *pr, const uint64_t *ix, int m, int pos,
                        uint64_t *sel, int sl, uint64_t cur, int cnt,
                        uint64_t *best, uint64_t base2, uint64_t p2)
{
    if (pos == m) {
        /* a single odd prime without factor 2 is a prime: excluded */
        if (cnt == 1 && p2 == 1) return;
        uint64_t d = cur * p2;
        if (d > *best) *best = d;
        return;
    }
    A327515_dfs(pr, ix, m, pos + 1, sel, sl, cur, cnt, best, base2, p2);
    bool ok = true;
    for (int k = 0; k < sl; ++k) {
        if (gcd_u64(ix[pos], sel[k]) != 1) { ok = false; break; }
    }
    if (ok) {
        sel[sl] = ix[pos];
        A327515_dfs(pr, ix, m, pos + 1, sel, sl + 1, cur * pr[pos], cnt + 1,
                    best, base2, p2);
    }
}

static uint64_t A327515_dmax(uint64_t n)
{
    uint64_t pr[64], ix[64], sel[64];
    int m = 0;
    uint64_t p2 = 1;
    uint64_t x = n;
    while ((x & 1ULL) == 0 && x > 0) { x >>= 1; p2 <<= 1; }
    for (uint64_t p = 3; p * p <= x; p += 2) {
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            pr[m++] = p;
        }
    }
    if (x > 1) pr[m++] = x;
    for (int i = 0; i < m; ++i) ix[i] = prime_pi_u64(pr[i]);
    uint64_t best = 1;
    A327515_dfs(pr, ix, m, 0, sel, 0, 1, 0, &best, 1, p2);
    return best;
}

seq_t a(idx_t n)
{
    uint64_t x = (uint64_t)n;
    seq_t steps = 0;
    while (x > 1) {
        uint64_t d = A327515_dmax(x);
        if (d <= 1) break;
        x /= d;
        ++steps;
    }
    return steps;
}
