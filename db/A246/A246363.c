/*
 * A246363  Permutation of natural numbers: a(n) = A135141(A048673(n)).
 * https://oeis.org/A246363
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A135141(A048673(n)); A048673(n) = (A003961(n)+1)/2;
 *            A135141(1)=1, A135141(p_k)=2*A135141(k),
 *            A135141(c_k)=2*A135141(k)+1 (p_k = k-th prime, c_k = k-th
 *            composite)
 * Range:     u8 n <= 23, u16 n <= 127, u32 n <= 971, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 91 terms through n = 9963 (2026-11-07)
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

static uint32_t *g_pi = NULL;
static size_t g_lim = 0;

static void g_build(size_t limit) {
    free(g_pi);
    g_pi = (uint32_t *)malloc((limit + 1) * sizeof(uint32_t));
    unsigned char *comp = (unsigned char *)malloc(limit + 1);
    for (size_t i = 0; i <= limit; ++i) comp[i] = 0;
    for (size_t p = 2; p * p <= limit; ++p)
        if (!comp[p])
            for (size_t x = p * p; x <= limit; x += p) comp[x] = 1;
    g_pi[0] = 0;
    if (limit >= 1) g_pi[1] = 0;
    for (size_t i = 2; i <= limit; ++i)
        g_pi[i] = g_pi[i - 1] + (comp[i] ? 0 : 1);
    free(comp);
    g_lim = limit;
}

static seq_t a135141_chain(uint64_t m) {
    if (m <= 1) return (seq_t)1;
    if ((size_t)m > g_lim) {
        size_t nl = g_lim ? g_lim : 1024;
        while (nl < (size_t)m) nl *= 2;
        g_build(nl);
    }
    unsigned char *bits = (unsigned char *)malloc((size_t)m + 1);
    size_t cnt = 0;
    size_t cur = (size_t)m;
    while (cur > 1) {
        size_t pi = g_pi[cur];
        bool prime = (g_pi[cur] != g_pi[cur - 1]);
        if (prime) {
            bits[cnt++] = 0;
            cur = pi;
        } else {
            bits[cnt++] = 1;
            cur = cur - pi - 1;
        }
    }
    seq_t r = (seq_t)1;
    while (cnt > 0) {
        --cnt;
        r = r * (seq_t)2 + (seq_t)bits[cnt];
    }
    free(bits);
    return r;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);
    uint64_t prod = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t np = next_prime(primes[i]);
        for (uint64_t k = 0; k < exps[i]; ++k) prod *= np;
    }
    uint64_t m = (prod + 1) / 2;
    return a135141_chain(m);
}
