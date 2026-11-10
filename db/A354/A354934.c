/*
 * A354934  Row 4 of A354940: Numbers k for which A345992(k) = 4, divided by
 *          4.
 * https://oeis.org/A354934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   search: a(n) = n-th k such that A345992(4*k) = 4, where
 *            A345992(N) = gcd(N, m), m = A344005(N) = least m>0 with N |
 *            m*(m+1)
 * Range:     u8 n <= 42, u16 n <= 5538, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 84 terms through n = 9396 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

static uint64_t A354934_gcd(uint64_t x, uint64_t y) {
    while (y) { uint64_t t = x % y; x = y; y = t; }
    return x;
}

static uint64_t A354934_inv(uint64_t a, uint64_t mod) {
    int64_t r0 = (int64_t)mod, r1 = (int64_t)(a % mod);
    int64_t t0 = 0, t1 = 1;
    if (mod == 1) return 0;
    while (r1 != 0) {
        int64_t q = r0 / r1, t;
        t = r0 - q * r1; r0 = r1; r1 = t;
        t = t0 - q * t1; t0 = t1; t1 = t;
    }
    if (t0 < 0) t0 += (int64_t)mod;
    return (uint64_t)t0;
}

/* A345992(N): idempotents e_i (≡1 mod q_i, ≡0 mod others); x = sum over subset
   satisfies x ≡ 0/1 mod each q_i, so m = x - 1 ≡ -1/0, i.e. N | m(m+1). */
static uint64_t A354934_a345992(uint64_t N) {
    uint64_t q[20], e[20];
    int c = 0;
    uint64_t t = N;
    for (uint64_t p = 2; p * p <= t; ++p) {
        if (t % p == 0) {
            uint64_t pe = 1;
            while (t % p == 0) { t /= p; pe *= p; }
            q[c++] = pe;
        }
    }
    if (t > 1) q[c++] = t;
    if (c == 0) return 1;
    for (int i = 0; i < c; ++i) {
        uint64_t o = N / q[i];
        uint64_t inv = A354934_inv(o % q[i], q[i]);
        e[i] = o * inv % N;
    }
    uint64_t best = N;
    for (uint64_t mask = 0; mask < ((uint64_t)1 << c); ++mask) {
        uint64_t x = 0;
        for (int i = 0; i < c; ++i)
            if (mask & ((uint64_t)1 << i)) x = (x + e[i]) % N;
        uint64_t m = (x + N - 1) % N;
        if (m == 0) m = N;
        if (m < best) best = m;
    }
    return A354934_gcd(N, best);
}

static uint64_t *A354934_terms = NULL;
static size_t A354934_cnt = 0, A354934_cap = 0;
static uint64_t A354934_next_k = 1;

seq_t a(idx_t n) {
    while ((uint64_t)A354934_cnt < (uint64_t)n) {
        uint64_t k = A354934_next_k;
        A354934_next_k += 2; /* even k are impossible */
        if (A354934_a345992(4 * k) == 4) {
            if (A354934_cnt == A354934_cap) {
                size_t nc = A354934_cap ? A354934_cap * 2 : 1024;
                uint64_t *p = (uint64_t *)realloc(A354934_terms, nc * sizeof(uint64_t));
                if (!p) return 0;
                A354934_terms = p;
                A354934_cap = nc;
            }
            A354934_terms[A354934_cnt++] = k;
        }
    }
    return (seq_t)A354934_terms[n - 1];
}
