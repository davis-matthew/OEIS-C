/*
 * A324521  Numbers > 1 where the maximum prime index is less than or equal to
 *          the number of prime factors counted with multiplicity.
 * https://oeis.org/A324521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 1664, u32 n <= 10929, u64 n <= 10929,
 *            u128 n <= 10929, big any n
 * Validated: standard, pass, 74 terms through n = 9422 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A324521: numbers m > 1 whose largest prime factor p has prime index
 * pi(p) <= Omega(m), the number of prime factors with multiplicity. A
 * smallest-prime-factor sieve and a prime-counting table up to a limit L give
 * both; L doubles until the term asked for is found (the original sieved to a
 * fixed 2000000 and the sequence passes it at n = 10929, a(10929) = 2^21). */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *terms = NULL;
static size_t term_len = 0;

static void build(uint64_t L)
{
    uint32_t *spf = (uint32_t *)calloc((size_t)L + 1, sizeof *spf);
    uint32_t *pi = (uint32_t *)malloc(((size_t)L + 1) * sizeof *pi);
    if (!spf || !pi) abort();
    for (uint64_t i = 2; i <= L; ++i)
        if (!spf[i])
            for (uint64_t j = i; j <= L; j += i)
                if (!spf[j]) spf[j] = (uint32_t)i;
    pi[0] = pi[1] = 0;
    for (uint64_t i = 2; i <= L; ++i) pi[i] = pi[i - 1] + (spf[i] == i);
    size_t cnt = 0, cap = 1024;
    uint64_t *t = (uint64_t *)malloc(cap * sizeof *t);
    if (!t) abort();
    for (uint64_t m = 2; m <= L; ++m) {
        uint64_t x = m, omega = 0, gpf = 1;
        while (x > 1) {
            uint64_t p = spf[x];
            x /= p;
            ++omega;
            if (p > gpf) gpf = p;
        }
        if (pi[gpf] <= omega) {
            if (cnt == cap) {
                uint64_t *u = (uint64_t *)realloc(t, (cap *= 2) * sizeof *t);
                if (!u) abort();
                t = u;
            }
            t[cnt++] = m;
        }
    }
    free(spf);
    free(pi);
    free(terms);
    terms = t;
    term_len = cnt;
}

seq_t a(idx_t n)
{
    static uint64_t L = 0;
    if (n < 1) return (seq_t)0;
    while (term_len < (size_t)n) {
        L = L ? 2 * L : 1 << 16;
        if (L > (UINT64_C(1) << 31)) return (seq_t)0;
        build(L);
    }
    return (seq_t)terms[n - 1];
}
