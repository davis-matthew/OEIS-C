/*
 * A324286  a(n) = A002487(A048675(n)).
 * https://oeis.org/A324286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002487(A048675(n)), where A048675(n) = (1/2)*Sum_{p^e ||
 *            n} e*2^PrimePi(p), and A002487 is Stern's diatomic series:
 *            a(0)=0, a(1)=1, a(2n)=a(n), a(2n+1)=a(n)+a(n+1)
 * Range:     u8 n <= 3237, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9887 (2026-11-08)
 * Helpers:   factor_u64, prime_pi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/prime_pi.h"

static void A324286_addbit(uint32_t *L, size_t pos) {
    size_t i = pos / 32;
    uint32_t add = (uint32_t)1 << (pos % 32);
    while (1) {
        uint32_t old = L[i];
        L[i] = old + add;
        if (L[i] >= old && (old + add) >= add) break;
        add = 1;
        i++;
    }
}

seq_t a(idx_t n) {
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    size_t pis[64];
    size_t pmax = 1;
    for (size_t i = 0; i < cnt; ++i) {
        pis[i] = (size_t)(long long)prime_pi((idx_t)primes[i]);
        if (pis[i] > pmax) pmax = pis[i];
    }
    size_t nl = (pmax + 80) / 32 + 2;
    uint32_t *L = (uint32_t *)calloc(nl, sizeof(uint32_t));
    if (!L) return 0;
    for (size_t i = 0; i < cnt; ++i) {
        size_t s = pis[i] - 1;
        uint64_t e = exps[i];
        for (int j = 0; j < 64; ++j)
            if ((e >> j) & 1) A324286_addbit(L, s + (size_t)j);
    }
    /* find top bit */
    long long top = (long long)nl * 32 - 1;
    while (top >= 0 && !((L[top / 32] >> (top % 32)) & 1)) --top;

    seq_t x = 0, y = 1; /* (stern(k), stern(k+1)) with k = 0 */
    for (long long b = top; b >= 0; --b) {
        if ((L[b / 32] >> (b % 32)) & 1) {
            x = x + y;
        } else {
            y = x + y;
        }
    }
    free(L);
    return x;
}
