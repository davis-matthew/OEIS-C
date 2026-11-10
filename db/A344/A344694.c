/*
 * A344694  Numbers k, not powers of primes, for which A011772(k) divides
 *          A344875(k), and for all proper divisors d of k, A011772(d) <
 *          A011772(k).
 * https://oeis.org/A344694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k (increasing) that is not a prime power, with
 *            A011772(k) | A344875(k) and A011772(k/p) < A011772(k) for every
 *            prime p | k; found by a search over k, computing A011772 via CRT
 *            over the prime-power factors of 2k
 * Range:     u8 none, u16 n <= 14, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static int64_t cand_egcd_inv(int64_t a, int64_t m)
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

static int cand_factor(uint64_t k, uint64_t *pe, uint64_t *p, int *e)
{
    int c = 0;
    for (uint64_t d = 2; d * d <= k; d += (d == 2 ? 1 : 2)) {
        if (k % d == 0) {
            int ex = 0;
            uint64_t pw = 1;
            while (k % d == 0) { k /= d; ex++; pw *= d; }
            p[c] = d; e[c] = ex; pe[c] = pw; c++;
        }
    }
    if (k > 1) { p[c] = k; e[c] = 1; pe[c] = k; c++; }
    return c;
}

/* A011772 via CRT */
static uint64_t cand_S(uint64_t k)
{
    uint64_t pe[20], p[20];
    int e[20];
    int c = cand_factor(k, pe, p, e);
    uint64_t q[21];
    int w = 0;
    bool has2 = false;
    for (int i = 0; i < c; i++) {
        if (p[i] == 2) { q[w++] = pe[i] * 2; has2 = true; }
        else q[w++] = pe[i];
    }
    if (!has2) q[w++] = 2;
    uint64_t N = 2 * k;
    uint64_t best = N - 1;
    for (int mask = 1; mask < (1 << w); mask++) {
        uint64_t A = 1;
        for (int i = 0; i < w; i++) if (mask >> i & 1) A *= q[i];
        uint64_t B = N / A;
        uint64_t m;
        if (B == 1) m = A;
        else {
            int64_t inv = cand_egcd_inv((int64_t)(A % B), (int64_t)B);
            uint64_t t = (B - (uint64_t)inv) % B;
            m = A * t;
        }
        if (m > 0 && m < best) best = m;
    }
    return best;
}

static uint64_t cand_f(const uint64_t *p, const int *e, const uint64_t *pe, int c)
{
    uint64_t r = 1;
    for (int i = 0; i < c; i++) {
        if (p[i] == 2) r *= (pe[i] * 2 - 1);
        else r *= (pe[i] - 1);
    }
    return r;
}

#define CAND_CAP 4096
static uint64_t cand_terms[CAND_CAP];
static idx_t cand_count = 0;
static uint64_t cand_next = 2;

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t k = cand_next;
    uint64_t found_val = 0;
    idx_t cnt = cand_count;
    if (n <= cand_count) return (seq_t)cand_terms[n - 1];
    for (;; k++) {
        uint64_t pe[20], p[20];
        int e[20];
        int c = cand_factor(k, pe, p, e);
        if (c < 2) continue;
        uint64_t f = cand_f(p, e, pe, c);
        uint64_t s = cand_S(k);
        if (f % s != 0) continue;
        bool ok = true;
        for (int i = 0; i < c && ok; i++) {
            if (cand_S(k / p[i]) >= s) ok = false;
        }
        if (!ok) continue;
        cnt++;
        if (cnt <= CAND_CAP) {
            cand_terms[cnt - 1] = k;
            cand_count = cnt;
            cand_next = k + 1;
        }
        if (cnt == n) { found_val = k; break; }
    }
    return (seq_t)found_val;
}
