/*
 * A318672  Denominators of the sequence whose Dirichlet convolution with
 *          itself yields A049599, number of (1+e)-divisors of n.
 * https://oeis.org/A318672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of f(n), f multiplicative Dirichlet square
 *            root of A049599; for prime powers g(0)=1, g(k) = (1/2)*(tau(k)+1
 *            - Sum_{i=1..k-1} g(i)*g(k-i)); a(n) = 2^(sum of 2-exponents of
 *            g(e_p))
 * Range:     u8 n <= 16385, u16 n <= 16385, u32 n <= 16385, u64 n <= 16385,
 *            u128 n <= 16385, big any n
 * Validated: standard, pass, 121 terms through n = 8949 (2026-11-08)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

typedef struct { __int128 num; int e; } A318672_rat;

static void A318672_compute(A318672_rat *g, int K) {
    g[0].num = 1; g[0].e = 0;
    for (int k = 1; k <= K; ++k) {
        int E = 0;
        for (int i = 1; i < k; ++i) {
            int ee = g[i].e + g[k - i].e;
            if (ee > E) E = ee;
        }
        int tau = 0;
        for (int d = 1; d <= k; ++d) if (k % d == 0) ++tau;
        __int128 t = (__int128)(tau + 1) << E;
        for (int i = 1; i < k; ++i) {
            int ee = g[i].e + g[k - i].e;
            t -= (g[i].num * g[k - i].num) << (E - ee);
        }
        int e = E + 1;
        while (e > 0 && (t % 2) == 0) { t /= 2; --e; }
        g[k].num = t;
        g[k].e = e;
    }
}

seq_t a(idx_t n) {
    if (n <= 1) return 1;
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    static A318672_rat g[70];
    static bool init = false;
    if (!init) { A318672_compute(g, 64); init = true; }
    long long total = 0;
    for (size_t i = 0; i < cnt; ++i) total += g[exps[i]].e;
    seq_t r = 1;
    for (long long j = 0; j < total; ++j) r *= 2;
    return r;
}
