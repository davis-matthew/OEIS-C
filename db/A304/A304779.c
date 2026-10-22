/*
 * A304779  The "rootless" zeta function. Dirichlet inverse of the function
 *          defined by r(n) = (-1)^Omega(n) if n is 1 or not a perfect power
 *          and r(n) = 0 otherwise.
 * https://oeis.org/A304779
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -Sum_{d|n, d>1, d not a perfect power} (-1)^Ω(d) * a(n/d)
 * Range:     u8 n <= 9802, u16 n <= 9802, u32 n <= 5519, u64 n <= 9802,
 *            u128 n <= 9802, big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/factor_u64.h"

/* ---------- Memoisation infrastructure ---------- */
static seq_t *a_cache      = NULL;
static bool  *a_computed   = NULL;
static idx_t  a_cap        = 0;

static void
ensure_cache_capacity(idx_t n)
{
    if (n < a_cap) return;
    idx_t new_cap = a_cap ? a_cap * 2 : 16;
    while (new_cap <= n) new_cap <<= 1;
    a_cache     = (seq_t *)realloc(a_cache,     new_cap * sizeof(seq_t));
    a_computed  = (bool  *)realloc(a_computed,  new_cap * sizeof(bool));
    for (idx_t i = a_cap; i < new_cap; ++i) {
        a_cache[i]    = 0;
        a_computed[i] = false;
    }
    a_cap = new_cap;
}

/* ---------- perfect‑power detection (1 is a perfect power) ---------- */
static bool
is_perfect_power(idx_t x)
{
    if (x <= 1) return true;          /* 1 is considered a perfect power */
    for (int e = 2; e <= 64; ++e) {
        long double root = pow((long double)x, 1.0L / e);
        uint64_t r = (uint64_t)(root + 0.5L);
        if (r < 2) continue;
        __int128 val = 1;
        for (int k = 0; k < e; ++k) {
            val *= r;
            if (val > x) break;
        }
        if (val == x) return true;
    }
    return false;
}

/* ---------- forward declaration of recursive routine ---------- */
static seq_t a_rec(idx_t n);

/* ---------- divisor enumeration to accumulate sum ---------- */
static void
enumerate_divisors(size_t idx,
                   uint64_t cur,
                   int omega,
                   const uint64_t *primes,
                   const uint64_t *exps,
                   size_t cnt,
                   idx_t n,
                   seq_t *sum)
{
    if (idx == cnt) {
        if (cur > 1 && !is_perfect_power(cur)) {
            int sign = (omega & 1) ? -1 : 1;
            seq_t t = a_rec(n / cur);
            if (sign == 1) *sum += t; else *sum -= t;
        }
        return;
    }
    uint64_t p = primes[idx];
    uint64_t mult = 1;
    for (unsigned e = 0; e <= exps[idx]; ++e) {
        enumerate_divisors(idx + 1, cur * mult, omega + (int)e,
                           primes, exps, cnt, n, sum);
        mult *= p;
    }
}

/* ---------- recursive calculation ---------- */
static seq_t
a_rec(idx_t n)
{
    if (n == 1) return 1;
    if (a_computed && a_computed[n]) return a_cache[n];

    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64(n, primes, exps, &cnt);

    seq_t sum = 0;
    enumerate_divisors(0, 1, 0, primes, exps, cnt, n, &sum);

    seq_t result = -sum;   /* unsigned wrap handles the minus */

    ensure_cache_capacity(n);
    a_cache[n]    = result;
    a_computed[n] = true;
    return result;
}

/* ---------- public interface ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* safety guard */
    if (a_cache == NULL) {
        ensure_cache_capacity(1);
        a_cache[1]    = 1;
        a_computed[1] = true;
    }
    return a_rec(n);
}
