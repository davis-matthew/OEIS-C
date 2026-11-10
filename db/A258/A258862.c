/*
 * A258862  Second pi-based antiderivative of n: the least m such that
 *          A258851^2(m) equals n.
 * https://oeis.org/A258862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min { m >= 0 : D(D(m)) = n }, D = A258851 (pi-based
 *            arithmetic derivative); computed by a search over m using a
 *            smallest-prime-factor sieve
 * Range:     u8 n <= 16, u16 n <= 861, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9172 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *A258862_spf = NULL;
static uint32_t *A258862_pi = NULL;
static uint64_t A258862_S = 0;

static void A258862_ensure(uint64_t x)
{
    uint64_t S, i, j;
    uint32_t cnt;
    uint32_t *spf, *pi;
    if (x <= A258862_S) return;
    S = A258862_S < 65536 ? 65536 : A258862_S * 2;
    while (S < x) S *= 2;
    spf = (uint32_t *)calloc((size_t)S + 1, sizeof(uint32_t));
    pi = (uint32_t *)malloc(((size_t)S + 1) * sizeof(uint32_t));
    if (!spf || !pi) { free(spf); free(pi); return; }
    for (i = 2; i <= S; i++) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (i * i <= S)
                for (j = i * i; j <= S; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    cnt = 0;
    pi[0] = 0;
    if (S >= 1) pi[1] = 0;
    for (i = 2; i <= S; i++) {
        if (spf[i] == i) cnt++;
        pi[i] = cnt;
    }
    free(A258862_spf);
    free(A258862_pi);
    A258862_spf = spf;
    A258862_pi = pi;
    A258862_S = S;
}

static uint64_t A258862_D(uint64_t v)
{
    uint64_t res = 0, m = v;
    if (v <= 1) return 0;
    A258862_ensure(v);
    while (m > 1) {
        uint64_t p = A258862_spf[m];
        uint64_t e = 0;
        while (m % p == 0) { m /= p; e++; }
        res += e * (uint64_t)A258862_pi[p] * (v / p);
    }
    return res;
}

seq_t a(idx_t n)
{
    uint64_t m;
    if (n == 0) return (seq_t)0;
    for (m = 0;; m++) {
        uint64_t d = A258862_D(m);
        if ((idx_t)A258862_D(d) == n) return (seq_t)m;
    }
}
