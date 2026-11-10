/*
 * A341615  Numbers k for which sigma(k) < 2k < A003961(k).
 * https://oeis.org/A341615
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with sigma(k) < 2k < A003961(k); computed by a
 *            smallest-prime-factor sieve (limit doubled as needed),
 *            evaluating sigma and A003961 from each factorization
 * Range:     u8 n <= 71, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9696 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 A341615_u128;

static uint64_t *A341615_terms = NULL;
static size_t A341615_len = 0;

static void A341615_build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 2, sizeof *spf);
    uint32_t *nxt = (uint32_t *)calloc((size_t)L + 2, sizeof *nxt);
    if (!spf || !nxt) abort();
    for (uint64_t i = 2; i <= L; ++i)
        if (!spf[i])
            for (uint64_t j = i; j <= L; j += i)
                if (!spf[j]) spf[j] = (uint32_t)i;
    uint64_t last = 0;
    for (uint64_t i = 2; i <= L; ++i)
        if (spf[i] == i) {
            if (last) nxt[last] = (uint32_t)i;
            last = i;
        }
    for (uint64_t q = last + 1;; ++q) {
        int prime = 1;
        for (uint64_t d = 2; d * d <= q; ++d) if (q % d == 0) { prime = 0; break; }
        if (prime) { nxt[last] = (uint32_t)q; break; }
    }
    size_t cnt = 0, cap = 1024;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof *t);
    if (!t) abort();
    for (uint64_t k = 2; k <= L; ++k) {
        uint64_t x = k;
        A341615_u128 f = 1, sg = 1;
        while (x > 1) {
            uint64_t p = spf[x];
            A341615_u128 term = 1, pw = 1;
            while (x % p == 0) {
                x /= p;
                f *= nxt[p];
                pw *= p;
                term += pw;
            }
            sg *= term;
        }
        if (sg < (A341615_u128)2 * k && f > (A341615_u128)2 * k) {
            if (cnt == cap) {
                uint64_t *u = (uint64_t *)realloc(t, (cap *= 2) * sizeof *t);
                if (!u) abort();
                t = u;
            }
            t[cnt++] = k;
        }
    }
    free(spf);
    free(nxt);
    free(A341615_terms);
    A341615_terms = t;
    A341615_len = cnt;
}

seq_t a(idx_t n)
{
    static uint64_t L = 0;
    if (n < 1) return (seq_t)0;
    while (A341615_len < (size_t)n) {
        L = L ? 2 * L : (uint64_t)1 << 16;
        if (L > ((uint64_t)1 << 27)) return (seq_t)0;
        A341615_build(L);
    }
    return (seq_t)A341615_terms[n - 1];
}
