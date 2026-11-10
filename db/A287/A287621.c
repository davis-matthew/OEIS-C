/*
 * A287621  a(n) = least k such that A287620(k) = n.
 * https://oeis.org/A287621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. a(n) = min over leaf sets L (squarefree d | n) and
 *            unordered factorizations n/d = v_1*...*v_t (v_i > 1) of d *
 *            Prod_j q_j^a(v_j), where q_j are the smallest primes not
 *            dividing d, with the largest exponents a(v_j) on the smallest
 *            primes; a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-08)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *A287621_memo = 0;
static unsigned char *A287621_flag = 0;
static uint64_t A287621_cap = 0;

static bool A287621_isprime(uint64_t n)
{
    if (n < 2) return false;
    for (uint64_t d = 2; d * d <= n; d++)
        if (n % d == 0) return false;
    return true;
}

static seq_t A287621_mul(seq_t x, seq_t y, bool *ok)
{
    seq_t r = x * y;
    if (x != 0 && r / x != y) *ok = false;
    return r;
}

static bool A287621_solve(uint64_t n, seq_t *out);

/* returns false if the candidate overflowed or exceeded the current best */
static bool A287621_cost(uint64_t d, const seq_t *E, int t,
                         const seq_t *best, bool have, seq_t *res)
{
    bool ok = true;
    seq_t cost = (seq_t)d;
    if (have && cost > *best) return false;
    uint64_t q = 1;
    for (int j = 0; j < t; j++) {
        do { q++; } while (!A287621_isprime(q) || d % q == 0);
        seq_t r = 1, b = (seq_t)q, e = E[j];
        while (e > 0) {
            if ((e & 1) != 0) {
                r = A287621_mul(r, b, &ok);
                if (!ok || (have && r > *best)) return false;
            }
            e >>= 1;
            if (e > 0) {
                b = A287621_mul(b, b, &ok);
                if (!ok || (have && b > *best)) return false;
            }
        }
        cost = A287621_mul(cost, r, &ok);
        if (!ok || (have && cost > *best)) return false;
    }
    *res = cost;
    return true;
}

static void A287621_eval(uint64_t d, const uint64_t *f, int t,
                         seq_t *best, bool *have)
{
    seq_t *E = mem_new(seq_t, t > 0 ? t : 1);
    for (int j = 0; j < t; j++) {
        if (!A287621_solve(f[j], &E[j])) { mem_free(E); return; }
    }
    for (int i = 1; i < t; i++) {
        int p = i;
        while (p > 0 && E[p - 1] < E[p]) {
            seq_t tmp = E[p]; E[p] = E[p - 1]; E[p - 1] = tmp;
            p--;
        }
    }
    seq_t *c = mem_new(seq_t, 1);
    if (A287621_cost(d, E, t, best, *have, &c[0])) {
        if (!*have || c[0] < *best) { *best = c[0]; *have = true; }
    }
    mem_free(c);
    mem_free(E);
}

static void A287621_fact(uint64_t r, uint64_t maxf, uint64_t d, uint64_t n,
                         uint64_t *f, int t, seq_t *best, bool *have)
{
    if (r == 1) {
        if (d == 1 && t == 1 && f[0] == n) return; /* self-reference */
        A287621_eval(d, f, t, best, have);
        return;
    }
    for (uint64_t g = (r < maxf ? r : maxf); g >= 2; g--) {
        if (r % g == 0) {
            f[t] = g;
            A287621_fact(r / g, g, d, n, f, t + 1, best, have);
        }
    }
}

static bool A287621_solve(uint64_t n, seq_t *out)
{
    if (n <= 1) { *out = (seq_t)1; return true; }
    if (n < A287621_cap && A287621_flag[n]) {
        *out = A287621_memo[n];
        return A287621_flag[n] == 1;
    }
    uint64_t p[64];
    int x[64];
    int w = 0;
    uint64_t m = n;
    for (uint64_t q = 2; q * q <= m; q++) {
        if (m % q == 0) {
            p[w] = q; x[w] = 0;
            while (m % q == 0) { m /= q; x[w]++; }
            w++;
        }
    }
    if (m > 1) { p[w] = m; x[w] = 1; w++; }

    seq_t *best = mem_new(seq_t, 1);
    bool have = false;
    uint64_t f[128];

    /* initial cheap candidate: all distinct primes as leaves, rest as primes */
    {
        uint64_t d = 1;
        int t = 0;
        for (int i = 0; i < w; i++) {
            d *= p[i];
            for (int c = 1; c < x[i]; c++) f[t++] = p[i];
        }
        if (!(d == 1 && t == 1 && f[0] == n))
            A287621_eval(d, f, t, best, &have);
    }

    for (unsigned long mask = 0; mask < (1UL << w); mask++) {
        uint64_t d = 1;
        for (int i = 0; i < w; i++)
            if (mask & (1UL << i)) d *= p[i];
        uint64_t r = n / d;
        A287621_fact(r, r, d, n, f, 0, best, &have);
    }

    *out = best[0];
    mem_free(best);
    if (n < A287621_cap) {
        A287621_memo[n] = *out;
        A287621_flag[n] = have ? 1 : 2;
    }
    return have;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;
    if ((uint64_t)n < (1ULL << 20) && (uint64_t)n >= A287621_cap) {
        uint64_t nc = A287621_cap ? A287621_cap : 64;
        while (nc <= (uint64_t)n) nc *= 2;
        if (A287621_memo == 0) A287621_memo = mem_new(seq_t, nc);
        else A287621_memo = mem_resize(seq_t, A287621_memo, nc);
        A287621_flag = (unsigned char *)realloc(A287621_flag, (size_t)nc);
        for (uint64_t i = A287621_cap; i < nc; i++) A287621_flag[i] = 0;
        A287621_cap = nc;
    }
    seq_t r = 0;
    A287621_solve((uint64_t)n, &r);
    return r;
}
