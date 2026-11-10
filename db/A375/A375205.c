/*
 * A375205  PrimePi(greatest prime < sqrt(Q)) - PrimePi(greatest prime
 *          factor(Q) < sqrt(Q)), with Q = A082686(n).
 * https://oeis.org/A375205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = PrimePi(P1) - PrimePi(P2), Q = A082686(n), P1 = greatest
 *            prime < sqrt(Q), P2 = greatest prime factor of Q that is <
 *            sqrt(Q); computed by a search plus a sieve
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 8985 (2026-11-10)
 * Helpers:   is_prime, isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/isqrt.h"

static uint64_t *q_cache = NULL;
static size_t q_len = 0;
static size_t q_cap = 0;
static uint64_t q_next = 3;

/* nth odd composite non-square (A082686) */
static uint64_t q_get(idx_t n)
{
    size_t want = (size_t)n;
    if (q_cap < want) {
        size_t nc = q_cap ? q_cap : 64;
        while (nc < want) nc *= 2;
        q_cache = (uint64_t *)realloc(q_cache, nc * sizeof(uint64_t));
        q_cap = nc;
    }
    while (q_len < want) {
        uint64_t v = q_next;
        q_next += 2;
        if (is_prime((idx_t)v)) continue;
        uint64_t s = (uint64_t)isqrt((idx_t)v);
        if (s * s == v) continue;
        q_cache[q_len++] = v;
    }
    return q_cache[want - 1];
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    uint64_t Q = q_get(n);
    uint64_t s = (uint64_t)isqrt((idx_t)Q); /* Q non-square: floor(sqrt) */

    /* greatest prime factor of Q that is < sqrt(Q) */
    uint64_t best = 0;
    uint64_t m = Q;
    for (uint64_t d = 2; d * d <= m; d++) {
        if (m % d == 0) {
            while (m % d == 0) m /= d;
            if (d <= s && d > best) best = d;
        }
    }
    if (m > 1 && m <= s && m > best) best = m;

    /* count primes in (best, s] with a sieve */
    unsigned char *comp = (unsigned char *)calloc((size_t)s + 2, 1);
    uint64_t cnt = 0;
    for (uint64_t i = 2; i <= s; i++) {
        if (!comp[i]) {
            if (i > best) cnt++;
            for (uint64_t j = i * i; j <= s; j += i) comp[j] = 1;
        }
    }
    free(comp);
    return (seq_t)cnt;
}
