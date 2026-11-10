/*
 * A344001  Indices k such that A011772(k) is odd.
 * https://oeis.org/A344001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A011772(k) odd; A011772(k) = least m>=1 with
 *            2k | m(m+1), found by CRT over unitary splits of 2k; searched
 *            incrementally over k
 * Range:     u8 n <= 98, u16 n <= 7187, u32 n <= 7187, u64 n <= 7187,
 *            u128 n <= 7187, big any n
 * Validated: standard, pass, 82 terms through n = 6731 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int64_t cs_egcd_inv(int64_t a, int64_t m)
{
    int64_t g = m, x = 0, y = 1, aa = a % m;
    int64_t r0 = m, r1 = aa, s0 = 0, s1 = 1;
    (void)g; (void)x; (void)y;
    while (r1 != 0) {
        int64_t q = r0 / r1;
        int64_t t = r0 - q * r1; r0 = r1; r1 = t;
        t = s0 - q * s1; s0 = s1; s1 = t;
    }
    s0 %= m;
    if (s0 < 0) s0 += m;
    return s0;
}

static uint64_t cs_A011772(uint64_t k)
{
    uint64_t N = 2 * k, t = N;
    uint64_t q[24];
    int c = 0;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            uint64_t pp = 1;
            while (t % p == 0) { t /= p; pp *= p; }
            q[c++] = pp;
        }
    }
    if (t > 1) q[c++] = t;
    uint64_t best = N - 1;
    for (int mask = 0; mask < (1 << c); mask++) {
        uint64_t A = 1;
        for (int i = 0; i < c; i++) if (mask >> i & 1) A *= q[i];
        uint64_t B = N / A;
        uint64_t m;
        if (B == 1) m = N;
        else {
            int64_t inv = cs_egcd_inv((int64_t)(A % B), (int64_t)B);
            uint64_t tt = (B - (uint64_t)inv) % B;
            m = A * tt;
        }
        if (m >= 1 && m < best) best = m;
    }
    return best;
}

static idx_t *cs_terms = 0;
static size_t cs_cap = 0, cs_cnt = 0;
static uint64_t cs_next = 1;

seq_t a(idx_t n)
{
    while (cs_cnt < n) {
        uint64_t k = cs_next++;
        if (cs_A011772(k) & 1) {
            if (cs_cnt == cs_cap) {
                cs_cap = cs_cap ? cs_cap * 2 : 1024;
                cs_terms = (idx_t *)realloc(cs_terms, cs_cap * sizeof(idx_t));
            }
            cs_terms[cs_cnt++] = (idx_t)k;
        }
    }
    return (seq_t)cs_terms[n - 1];
}
