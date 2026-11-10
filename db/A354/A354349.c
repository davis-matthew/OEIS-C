/*
 * A354349  Dirichlet inverse of A181819, prime shadow of n.
 * https://oeis.org/A354349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1)=1, a(n) = -Sum_{d|n, d<n} A181819(n/d)*a(d);
 *            multiplicative, with a(p^k) = -Sum_{j=1..k} prime(j)*a(p^(k-j)),
 *            a(1)=1
 * Range:     i8 n <= 24575, i16 n <= 65537, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 107 terms through n = 9878 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

static void A354349_small_primes(unsigned *pr, int cnt)
{
    int sz = 0;
    unsigned c = 2;
    while (sz < cnt) {
        bool ok = true;
        for (int i = 0; i < sz; ++i) {
            if (pr[i] * pr[i] > c) break;
            if (c % pr[i] == 0) { ok = false; break; }
        }
        if (ok) pr[sz++] = c;
        ++c;
    }
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    unsigned pr[70];
    A354349_small_primes(pr, 70);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        int e = (int)exps[i];
        seq_t g[70];
        g[0] = (seq_t)1;
        for (int k = 1; k <= e; ++k) {
            seq_t s = (seq_t)0;
            for (int j = 1; j <= k; ++j)
                s += (seq_t)pr[j - 1] * g[k - j];
            g[k] = -s;
        }
        result = result * g[e];
    }
    return result;
}
