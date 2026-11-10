/*
 * A344980  Numbers k such that A011772(k) does not divide A344875(k).
 * https://oeis.org/A344980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 such that A011772(k) does not divide
 *            A344875(k); computed by sieving k upward (CRT for A011772,
 *            prime-power product for A344875)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 72 terms through n = 72 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static int64_t A344980_inv(int64_t a, int64_t m)
{
    int64_t g = m, x = 0, y = 1, aa = a % m;
    while (aa != 0) {
        int64_t q = g / aa, t = g - q * aa;
        g = aa; aa = t;
        t = x - q * y; x = y; y = t;
    }
    /* x * a ≡ g (mod m) with g == 1 */
    x %= m;
    if (x < 0) x += m;
    return x;
}

static int A344980_bad(uint64_t k)
{
    uint64_t N = 2 * k, tmp = N;
    uint64_t qs[24];
    int c = 0;
    uint64_t t = 1;
    for (uint64_t p = 2; p * p <= tmp; p += (p == 2 ? 1 : 2)) {
        if (tmp % p == 0) {
            uint64_t q = 1;
            while (tmp % p == 0) { tmp /= p; q *= p; }
            qs[c++] = q;
            t *= (q - 1);
        }
    }
    if (tmp > 1) { qs[c++] = tmp; t *= (tmp - 1); }

    uint64_t best = N - 1;
    for (uint32_t mask = 1; mask < (1u << c); mask++) {
        uint64_t d = 1;
        for (int i = 0; i < c; i++)
            if (mask & (1u << i)) d *= qs[i];
        uint64_t e = N / d;
        if (e == 1) continue;
        uint64_t inv = (uint64_t)A344980_inv((int64_t)(d % e), (int64_t)e);
        uint64_t mp = (e - inv) % e;
        uint64_t m = d * mp;
        if (m >= 1 && m < best) best = m;
    }
    return (t % best) != 0;
}

static uint64_t *A344980_terms = 0;
static size_t A344980_cnt = 0, A344980_cap = 0;
static uint64_t A344980_next = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A344980_cnt < (size_t)n) {
        uint64_t k = A344980_next++;
        if (A344980_bad(k)) {
            if (A344980_cnt == A344980_cap) {
                A344980_cap = A344980_cap ? A344980_cap * 2 : 256;
                A344980_terms = (uint64_t *)realloc(A344980_terms, A344980_cap * sizeof(uint64_t));
            }
            A344980_terms[A344980_cnt++] = k;
        }
    }
    return (seq_t)A344980_terms[n - 1];
}
