/*
 * A266227  a(n) = floor(Sum_{d|n} 1/sigma(d)).
 * https://oeis.org/A266227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Σ_{d|n} 1/σ(d) ) where σ is the sum of divisors
 * Range:     u8 n <= 16384, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 103 terms through n = 8676 (2026-11-03)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A266227: floor(Sum_{d|n} 1/sigma(d)). The sum is small, so it is added in
 * long double; only when it lies within 1e-9 of an integer is it recomputed
 * exactly, as a reduced 128-bit fraction. (The original summed fractions in
 * seq_t, whose denominators overflow at 64 bits.) */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/factor_u64.h"

typedef unsigned __int128 u128;

static u128 gcd128(u128 a, u128 b)
{
    while (b) { u128 t = a % b; a = b; b = t; }
    return a;
}

static uint64_t pr[64], ex[64];
static size_t np;

/* every divisor's sigma, by its prime powers: calls f(sigma(d)) */
static void each(size_t i, uint64_t s, void (*f)(uint64_t))
{
    if (i == np) { f(s); return; }
    uint64_t pe = 1, sig = 1;               /* p^e, sigma(p^e) */
    for (uint64_t e = 0; e <= ex[i]; ++e) {
        each(i + 1, s * sig, f);
        pe *= pr[i];
        sig += pe;
    }
}

static long double fsum;
static void add_float(uint64_t s) { fsum += 1.0L / (long double)s; }

static u128 qn, qd;
static int exact_ok;
static void add_exact(uint64_t s)
{
    u128 g = gcd128(qd, s);
    u128 l = qd / g;                        /* new denominator l * s */
    if (l > (~(u128)0) / s) { exact_ok = 0; return; }
    qn = qn * (s / g) + l;
    qd = l * s;
    g = gcd128(qn, qd);
    qn /= g;
    qd /= g;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    np = 0;
    factor_u64((uint64_t)n, pr, ex, &np);
    fsum = 0;
    each(0, 1, add_float);
    long double fl = floorl(fsum);
    if (fsum - fl > 1e-9L && fsum - fl < 1 - 1e-9L) return (seq_t)(uint64_t)fl;
    qn = 0;
    qd = 1;
    exact_ok = 1;
    each(0, 1, add_exact);
    if (!exact_ok) return (seq_t)(uint64_t)floorl(fsum + 1e-12L);
    return (seq_t)(uint64_t)(qn / qd);
}
