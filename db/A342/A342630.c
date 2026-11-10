/*
 * A342630  Numbers k such that A340180(k) is prime.
 * https://oeis.org/A342630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: k is a term iff A340180(k) is prime, where A340180(k) =
 *            Sum_{1<=x<k, gcd(x,k)=1} (sigma(k) mod x), computed by Moebius
 *            inclusion-exclusion over the squarefree divisors d of k:
 *            Sum_{d|k} mu(d) * Sum_{l=1..floor((k-1)/d)} (s -
 *            d*l*floor((s/d)/l)), s = sigma(k)
 * Range:     u8 n <= 28, u16 n <= 3621, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 79 terms through n = 4853 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A342630_val(uint64_t k) {
    if (k < 2) return 0;
    uint64_t pr[16];
    int np = 0;
    uint64_t sg = 1, m = k;
    for (uint64_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            uint64_t t = 1, pw = 1;
            while (m % p == 0) { m /= p; pw *= p; t += pw; }
            sg *= t;
            pr[np++] = p;
        }
    }
    if (m > 1) { sg *= (m + 1); pr[np++] = m; }
    uint64_t s = sg;
    uint64_t pos = 0, neg = 0;
    for (int mask = 0; mask < (1 << np); ++mask) {
        uint64_t d = 1;
        int bits = 0;
        for (int i = 0; i < np; ++i)
            if (mask >> i & 1) { d *= pr[i]; ++bits; }
        uint64_t M = (k - 1) / d;
        if (M == 0) continue;
        uint64_t s1 = s / d;
        uint64_t acc = 0;
        for (uint64_t l = 1; l <= M;) {
            uint64_t q = s1 / l;
            uint64_t r = s1 / q;
            if (r > M) r = M;
            acc += q * ((l + r) * (r - l + 1) / 2);
            l = r + 1;
        }
        uint64_t term = s * M - d * acc;
        if (bits & 1) neg += term; else pos += term;
    }
    return pos - neg;
}

static uint64_t *A342630_terms = 0;
static size_t A342630_cnt = 0, A342630_cap = 0;
static uint64_t A342630_next = 1;

seq_t a(idx_t n) {
    while (A342630_cnt < (size_t)n) {
        uint64_t k = A342630_next++;
        uint64_t v = A342630_val(k);
        if (v >= 2 && is_prime_u64(v)) {
            if (A342630_cnt == A342630_cap) {
                A342630_cap = A342630_cap ? A342630_cap * 2 : 1024;
                A342630_terms = (uint64_t *)realloc(A342630_terms, A342630_cap * sizeof(uint64_t));
            }
            A342630_terms[A342630_cnt++] = k;
        }
    }
    return (seq_t)A342630_terms[n - 1];
}
