/*
 * A394973  a(n) is the smallest k such that A091508(k) = n.
 * https://oeis.org/A394973
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   backward search. b(n) = 1 forces b(n-1) to be a divisor g > 1 of
 *            n. * In general, b(j) is either b(j+1) - (j+1), when gcd(j+1,
 *            b(j)) = 1, or * b(j+1)*g, where g > 1 divides j+1 and gcd(j+1,
 *            b(j)) = g. The code works * back from b(n) = 1 to b(1) = k and
 *            keeps the smallest k for which no * b(j) = 1...
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

static uint64_t A394973_best;
static uint64_t A394973_bound;

static uint64_t A394973_gcd_small(uint64_t m, uint64_t u)
{
    uint64_t x = m, y = u % m;
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

/* v = b(level); enumerate predecessors b(level-1) */
static void A394973_dfs(uint64_t level, uint64_t v)
{
    if (level == 1) {
        if (v < A394973_best) {
            A394973_best = v;
            if (v - 1 < A394973_bound) A394973_bound = v - 1;
        }
        return;
    }
    uint64_t j = level - 1;      /* predecessor index */
    uint64_t m = level;          /* = j + 1 */
    uint64_t B = A394973_bound;
    uint64_t extra = j * (j + 1) / 2 - 1;
    uint64_t lim = (B > UINT64_MAX - extra) ? UINT64_MAX : B + extra;

    /* division predecessors: u = v*g, g | m, g > 1, gcd(m,u) = g */
    for (uint64_t g = 2; g <= m; ++g) {
        if (m % g) continue;
        if (v > lim / g) continue;
        uint64_t u = v * g;
        if (A394973_gcd_small(m, u) == g) {
            A394973_dfs(j, u);
            B = A394973_bound;
            lim = (B > UINT64_MAX - extra) ? UINT64_MAX : B + extra;
        }
    }
    /* addition predecessor: u = v - m, gcd(m,u) = 1, u != 1 */
    if (v > m + 1) {
        uint64_t u = v - m;
        if (u <= lim && A394973_gcd_small(m, u) == 1)
            A394973_dfs(j, u);
    }
}

seq_t a(idx_t n)
{
    static uint64_t memo[512];
    static bool have[512];
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n < 512 && have[n]) return (seq_t)memo[n];

    uint64_t B = 64;
    uint64_t res = 0;
    for (;;) {
        A394973_best = UINT64_MAX;
        A394973_bound = B;
        A394973_dfs((uint64_t)n, 1);
        if (A394973_best != UINT64_MAX) { res = A394973_best; break; }
        if (B > (UINT64_MAX >> 4)) break;
        B <<= 3;
    }
    if (n < 512) { memo[n] = res; have[n] = true; }
    return (seq_t)res;
}
