/*
 * A260124  The second infinite sequence starting with a(0)=0 such that
 *          A(a(k)) = a(k-1) for all k>=1, where A(n) = n - A037445(n) (cf.
 *          A260084).
 * https://oeis.org/A260124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A260084(n)/2; computed by a depth-first search over
 *            preimages: a(0)=0, a(1)=1, A(a(k)) = a(k-1), where A(m) = m -
 *            A037445(m)
 * Range:     u8 n <= 76, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Validated: partial, pass, 92 terms through n = 91 (2026-11-07)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

#define A260124_MAXD 70000

static unsigned A260124_popc(uint64_t x)
{
    unsigned c = 0;
    while (x) { x &= x - 1; ++c; }
    return c;
}

static uint64_t A260124_dinf(uint64_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0, i;
    uint64_t res = 1;
    if (n <= 1) return 1;
    factor_u64(n, primes, exps, &cnt);
    for (i = 0; i < cnt; ++i)
        res <<= A260124_popc(exps[i]);
    return res;
}

static uint64_t A260124_isqrt(uint64_t m)
{
    uint64_t r = 0;
    uint64_t lo = 0, hi = (uint64_t)1 << 32;
    while (lo < hi) {
        uint64_t mid = lo + (hi - lo + 1) / 2;
        if (mid * mid <= m) lo = mid; else hi = mid - 1;
    }
    r = lo;
    return r;
}

static uint64_t A260124_path[A260124_MAXD + 2];
static uint64_t A260124_start[A260124_MAXD + 2];
static uint64_t A260124_have = 0; /* number of terms valid in path (0 = none) */

static void A260124_build(uint64_t D)
{
    uint64_t k = 1;
    A260124_path[0] = 0;
    A260124_start[1] = 1;
    while (k < D) {
        uint64_t prev = A260124_path[k - 1];
        uint64_t limit = prev + 2 * A260124_isqrt(prev + 1) + 6;
        uint64_t n = A260124_start[k];
        bool found = false;
        if (k == 1) {
            if (n <= 1) {
                A260124_path[1] = 1;
                A260124_start[1] = 2;
                found = true;
            }
        } else {
            for (; n <= limit; ++n) {
                if (n - A260124_dinf(n) == prev) {
                    A260124_path[k] = n;
                    A260124_start[k] = n + 1;
                    found = true;
                    break;
                }
            }
        }
        if (found) {
            ++k;
            A260124_start[k] = A260124_path[k - 1] + 1;
        } else {
            if (k == 1) break; /* should not happen */
            --k;
        }
    }
    A260124_have = k;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n >= A260124_have) {
        uint64_t D = (uint64_t)n + 41;
        if (D < 200) D = 200;
        if (D > A260124_MAXD) D = A260124_MAXD;
        A260124_build(D);
    }
    return (seq_t)A260124_path[n];
}
