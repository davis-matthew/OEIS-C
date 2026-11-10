/*
 * A276938  Second row of A276941: a(n) = A003961(A276937(n)).
 * https://oeis.org/A276938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003961(A276937(n)); A276937 found by sieve (m with every
 *            prime p_k having exponent <= k and at least one equal), then
 *            each term mapped by prime(k) -> prime(k+1)
 * Range:     u8 n <= 17, u16 n <= 1731, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 75 terms through n = 4907 (2026-11-08)
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/next_prime.h"

static uint32_t *A276938_terms = NULL;
static size_t A276938_cnt = 0;
static size_t A276938_limit = 0;

static void A276938_build(size_t limit)
{
    uint32_t *spf = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    uint32_t *pidx = (uint32_t*)calloc(limit + 1, sizeof(uint32_t));
    uint32_t *terms = (uint32_t*)malloc((limit / 2 + 16) * sizeof(uint32_t));
    size_t cnt = 0, i, j;
    uint32_t idx = 0;
    for (i = 0; i <= limit; ++i) spf[i] = (uint32_t)i;
    for (i = 2; i * i <= limit; ++i)
        if (spf[i] == i)
            for (j = i * i; j <= limit; j += i)
                if (spf[j] == j) spf[j] = (uint32_t)i;
    for (i = 2; i <= limit; ++i)
        if (spf[i] == i) pidx[i] = ++idx;
    for (i = 2; i <= limit; ++i) {
        size_t x = i;
        bool ok = true, eq = false;
        while (x > 1) {
            uint32_t p = spf[x], e = 0;
            while (x % p == 0) { x /= p; ++e; }
            if (e > pidx[p]) { ok = false; break; }
            if (e == pidx[p]) eq = true;
        }
        if (ok && eq) terms[cnt++] = (uint32_t)i;
    }
    free(spf);
    free(pidx);
    free(A276938_terms);
    A276938_terms = terms;
    A276938_cnt = cnt;
    A276938_limit = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (A276938_limit == 0) A276938_build((size_t)1 << 16);
    while ((size_t)n > A276938_cnt) A276938_build(A276938_limit * 2);
    uint64_t m = A276938_terms[n - 1];
    seq_t result = 1;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t q = next_prime(p);
            while (m % p == 0) {
                m /= p;
                result *= (seq_t)q;
            }
        }
    }
    if (m > 1) result *= (seq_t)next_prime(m);
    return result;
}
