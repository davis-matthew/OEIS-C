/*
 * A355692  Dirichlet inverse of A355442, gcd(A003961(n), A276086(n)), where
 *          A003961 is fully multiplicative with a(p) = nextprime(p), and
 *          A276086 is primorial base exp-function.
 * https://oeis.org/A355692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1, a(n) = -Sum_{d|n, d<n} A355442(n/d) * a(d), with
 *            A355442(m) = gcd(A003961(m), A276086(m))
 * Range:     i8 n <= 44, i16 n <= 719, i32 n <= 11550, i64 n <= 11550,
 *            i128 n <= 11550, big any n
 * Validated: standard, pass, 97 terms through n = 9727 (2026-11-10)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static const uint64_t A355692_pr[20] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71};

/* A355442(m) = prod_{k>=1} prime(k+1)^min(v_{prime(k)}(m), digit_k(m in primorial base)) */
static seq_t A355692_f(uint64_t m)
{
    uint64_t prim[17];
    int K = 0;
    prim[0] = 1;
    while (K < 15 && m / A355692_pr[K] >= prim[K]) {
        prim[K + 1] = prim[K] * A355692_pr[K];
        ++K;
    }
    int ex[17];
    uint64_t t = m;
    for (int i = 0; i < K; ++i) {
        int c = 0;
        while (t % A355692_pr[i] == 0) { t /= A355692_pr[i]; ++c; }
        ex[i] = c;
    }
    uint64_t dig[17];
    uint64_t rem = m;
    for (int i = K; i >= 0; --i) {
        dig[i] = rem / prim[i];
        rem -= dig[i] * prim[i];
    }
    seq_t result = 1;
    for (int k = 1; k <= K; ++k) {
        uint64_t e = (uint64_t)ex[k - 1];
        uint64_t mm = e < dig[k] ? e : dig[k];
        for (uint64_t j = 0; j < mm; ++j)
            result *= (seq_t)A355692_pr[k];
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    /* collect divisors */
    size_t cap = 64, cnt = 0;
    uint64_t *dv = (uint64_t *)malloc(cap * sizeof(uint64_t));
    if (!dv) abort();
    uint64_t N = (uint64_t)n;
    for (uint64_t i = 1; i * i <= N; ++i) {
        if (N % i == 0) {
            if (cnt + 2 > cap) {
                cap *= 2;
                dv = (uint64_t *)realloc(dv, cap * sizeof(uint64_t));
                if (!dv) abort();
            }
            dv[cnt++] = i;
            if (i != N / i) dv[cnt++] = N / i;
        }
    }
    /* insertion sort ascending */
    for (size_t i = 1; i < cnt; ++i) {
        uint64_t x = dv[i];
        size_t j = i;
        while (j > 0 && dv[j - 1] > x) { dv[j] = dv[j - 1]; --j; }
        dv[j] = x;
    }
    seq_t *av = mem_new(seq_t, cnt);
    for (size_t i = 0; i < cnt; ++i) {
        if (dv[i] == 1) { av[i] = 1; continue; }
        seq_t s = 0;
        for (size_t j = 0; j < i; ++j) {
            if (dv[i] % dv[j] == 0)
                s += A355692_f(dv[i] / dv[j]) * av[j];
        }
        av[i] = -s;
    }
    seq_t res = av[cnt - 1];
    mem_free(av);
    free(dv);
    return res;
}
