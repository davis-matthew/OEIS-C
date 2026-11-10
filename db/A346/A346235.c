/*
 * A346235  Dirichlet inverse of A341530, gcd(n*sigma(A003961(n)),
 *          sigma(n)*A003961(n)).
 * https://oeis.org/A346235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   Dirichlet inverse g of b(n)=A341530(n)=gcd(n*sigma(A003961(n)),
 *            sigma(n)*A003961(n)): g(1)=1, g(n) = -Sum_{d|n, d>1} b(d)*g(n/d)
 * Range:     i8 n <= 5, i16 n <= 47, i32 n <= 9999, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 94 terms through n = 9794 (2026-11-10)
 * Helpers:   factor_u64, gcd, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/gcd.h"
#include "../utils/next_prime.h"
#include "../utils/mem.h"

static seq_t A346235_b(uint64_t d)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64(d, primes, exps, &cnt);
    seq_t M = 1, sigM = 1, sigd = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t q = next_prime(p);
        seq_t qp = 1, pp = 1, sq = 1, sp = 1;
        for (uint64_t k = 0; k < exps[i]; ++k) {
            qp *= (seq_t)q;
            pp *= (seq_t)p;
            sq += qp;
            sp += pp;
        }
        M *= qp;
        sigM *= sq;
        sigd *= sp;
    }
    seq_t t1 = (seq_t)d * sigM;
    seq_t t2 = sigd * M;
    return gcd(t1, t2);
}

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    size_t cnt = 0;
    for (uint64_t d = 1; d * d <= N; ++d)
        if (N % d == 0) cnt += (d * d == N) ? 1 : 2;
    uint64_t *div = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    size_t k = 0;
    for (uint64_t d = 1; d * d <= N; ++d)
        if (N % d == 0) {
            div[k++] = d;
            if (d * d != N) div[k++] = N / d;
        }
    /* insertion sort */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t v = div[i];
        size_t j = i;
        while (j > 0 && div[j - 1] > v) { div[j] = div[j - 1]; --j; }
        div[j] = v;
    }
    seq_t *b = mem_new(seq_t, cnt);
    seq_t *g = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; ++i) b[i] = A346235_b(div[i]);
    g[0] = 1;
    for (size_t i = 1; i < cnt; ++i) {
        seq_t s = 0;
        for (size_t j = 1; j <= i; ++j) {
            if (div[i] % div[j] == 0) {
                uint64_t q = div[i] / div[j];
                size_t lo = 0, hi = i;
                while (lo < hi) {
                    size_t mid = (lo + hi) / 2;
                    if (div[mid] < q) lo = mid + 1; else hi = mid;
                }
                s += b[j] * g[lo];
            }
        }
        g[i] = -s;
    }
    seq_t res = g[cnt - 1];
    mem_free(b);
    mem_free(g);
    free(div);
    return res;
}
