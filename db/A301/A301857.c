/*
 * A301857  The lesser of j,k, the smallest pair such that A023900(j) +
 *          A023900(k) = 0 and j,k are coprime.
 * https://oeis.org/A301857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search. For each value v = 1, 4, 8, 12, ... in increasing order,
 *            let j be the smallest number with A023900(j) = +v and k the
 *            smallest with A023900(k) = -v. The pair is the minima of
 *            squarefree products of primes p with Product (p-1) = v, and the
 *            prime 2 flips the sign. If gcd(j,k) = 1, min(j,k) i...
 * Range:     u8 n <= 16, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *A301857_terms = 0;
static uint64_t A301857_tcnt = 0, A301857_tcap = 0;
static uint64_t A301857_lastv = 0;

static uint64_t A301857_cand[1024];
static int A301857_nc;
static uint64_t A301857_best[2];

static int A301857_isprime(uint64_t p)
{
    if (p < 2) return 0;
    if (p % 2 == 0) return p == 2;
    for (uint64_t d = 3; d * d <= p; d += 2)
        if (p % d == 0) return 0;
    return 1;
}

static uint64_t A301857_gcd(uint64_t a, uint64_t b)
{
    while (b) { uint64_t t = a % b; a = b; b = t; }
    return a;
}

static void A301857_upd(int par, uint64_t x)
{
    if (A301857_best[par] == 0 || x < A301857_best[par]) A301857_best[par] = x;
}

/* odd primes only; parity = number of odd primes; adding 2 flips sign, doubles product */
static void A301857_dfs(int idx, uint64_t rem, uint64_t prod, int par)
{
    if (rem == 1) {
        A301857_upd(par, prod);
        A301857_upd(par ^ 1, prod * 2);
        return;
    }
    for (int i = idx; i < A301857_nc; i++) {
        uint64_t d = A301857_cand[i] - 1;
        if (d > rem) break;
        if (rem % d == 0)
            A301857_dfs(i + 1, rem / d, prod * A301857_cand[i], par ^ 1);
    }
}

/* returns min(j,k) for value v if the minimal pair is coprime, else 0 */
static uint64_t A301857_solve(uint64_t v)
{
    A301857_nc = 0;
    for (uint64_t d = 1; d * d <= v; d++) {
        if (v % d) continue;
        uint64_t e[2];
        e[0] = d; e[1] = v / d;
        for (int t = 0; t < 2; t++) {
            if (t == 1 && e[1] == e[0]) continue;
            uint64_t dd = e[t];
            if (dd < 2 || (dd & 1)) continue;
            if (!A301857_isprime(dd + 1)) continue;
            if (A301857_nc < 1024) A301857_cand[A301857_nc++] = dd + 1;
        }
    }
    for (int i = 1; i < A301857_nc; i++) {
        uint64_t x = A301857_cand[i];
        int k = i - 1;
        while (k >= 0 && A301857_cand[k] > x) { A301857_cand[k + 1] = A301857_cand[k]; k--; }
        A301857_cand[k + 1] = x;
    }
    A301857_best[0] = A301857_best[1] = 0;
    A301857_dfs(0, v, 1, 0);
    uint64_t j = A301857_best[0], k = A301857_best[1];
    if (j == 0 || k == 0) return 0;
    if (A301857_gcd(j, k) != 1) return 0;
    return j < k ? j : k;
}

static void A301857_add(uint64_t t)
{
    if (A301857_tcnt >= A301857_tcap) {
        A301857_tcap = A301857_tcap ? A301857_tcap * 2 : 256;
        A301857_terms = (uint64_t *)realloc(A301857_terms, A301857_tcap * sizeof(uint64_t));
    }
    A301857_terms[A301857_tcnt++] = t;
}

seq_t a(idx_t n)
{
    while (A301857_tcnt < (uint64_t)n) {
        if (A301857_lastv == 0) A301857_lastv = 1;
        else if (A301857_lastv == 1) A301857_lastv = 4;
        else A301857_lastv += 4;
        uint64_t r = A301857_solve(A301857_lastv);
        if (r) A301857_add(r);
    }
    return (seq_t)A301857_terms[(uint64_t)n - 1];
}
