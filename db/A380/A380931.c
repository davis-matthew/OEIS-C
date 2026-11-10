/*
 * A380931  Numbers k such that A380845(k) > 4*k.
 * https://oeis.org/A380931
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search, not a closed form. a(n) is the n-th k such that
 *            A380845(k) > 4*k, where A380845(k) = Sum_{d|k,
 *            popcount(d)=popcount(k)} d. Because A380845(k) <= sigma(k),
 *            every term is 4-abundant (sigma(k) > 4k). A DFS over prime
 *            factorizations (bounded by the maximum abundancy still
 *            reachable) lists the...
 * Range:     u8 none, u16 none, u32 n <= 307, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 998 (2026-11-10)
 * Helpers:   is_prime_u64, popcount
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/popcount.h"
#include "../utils/is_prime_u64.h"

static uint32_t *A380931_P = NULL;
static size_t A380931_NP = 0;
static uint64_t A380931_PLIM = 0;
static uint64_t A380931_N = 0;
static uint64_t *A380931_res = NULL;
static size_t A380931_nres = 0, A380931_cap = 0;
static bool A380931_done = false;
static uint64_t A380931_fp[40], A380931_fe[40];
static int A380931_fd = 0;

/* state for the staged divisor check */
static uint64_t A380931_k, A380931_D, A380931_all, A380931_match;
static int A380931_pc;

static void A380931_sieve(uint64_t lim)
{
    if (lim <= A380931_PLIM) return;
    char *comp = (char *)calloc((size_t)lim + 1, 1);
    free(A380931_P);
    size_t cap = 1024;
    A380931_P = (uint32_t *)malloc(cap * sizeof(uint32_t));
    A380931_NP = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!comp[i]) {
            if (A380931_NP == cap) {
                cap *= 2;
                A380931_P = (uint32_t *)realloc(A380931_P, cap * sizeof(uint32_t));
            }
            A380931_P[A380931_NP++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;
        }
    }
    free(comp);
    A380931_PLIM = lim;
}

/* enumerate divisors d <= D of k; accumulate codivisors e = k/d */
static void A380931_rec(int depth, uint64_t d)
{
    if (depth == A380931_fd) {
        uint64_t e = A380931_k / d;
        A380931_all += e;
        if (popcount(e) == A380931_pc) A380931_match += e;
        return;
    }
    uint64_t p = A380931_fp[depth];
    uint64_t emax = A380931_fe[depth];
    for (uint64_t ex = 0;; ex++) {
        A380931_rec(depth + 1, d);
        if (ex == emax) break;
        if (d > A380931_D / p) break;
        d *= p;
    }
}

static void A380931_check(uint64_t k, uint64_t sigma)
{
    static const uint64_t stages[6] = {8ULL, 64ULL, 512ULL, 4096ULL, 65536ULL, UINT64_MAX};
    uint64_t target = k * 4;
    bool ok = false;
    A380931_k = k;
    A380931_pc = popcount(k);
    for (int s = 0; s < 6; s++) {
        A380931_D = stages[s];
        A380931_all = 0;
        A380931_match = 0;
        A380931_rec(0, 1);
        if (A380931_match > target) { ok = true; break; }
        /* the codivisors not yet seen sum to sigma - all at most */
        if (A380931_match + (sigma - A380931_all) <= target) { ok = false; break; }
    }
    if (ok) {
        if (A380931_nres == A380931_cap) {
            A380931_cap = A380931_cap ? A380931_cap * 2 : 256;
            A380931_res = (uint64_t *)realloc(A380931_res, A380931_cap * sizeof(uint64_t));
        }
        A380931_res[A380931_nres++] = k;
    }
}

static void A380931_dfs(size_t j, uint64_t m, uint64_t sig)
{
    uint64_t rem = A380931_N / m;
    double ab = (double)sig / (double)m;
    for (size_t i = j; i < A380931_NP; i++) {
        uint64_t p = A380931_P[i];
        if (p > rem) return;
        /* upper bound on the abundancy gain from primes >= p that fit in rem */
        double f = 1.0;
        uint64_t prod = 1;
        size_t t = i;
        bool inf = false;
        for (;;) {
            if (t >= A380931_NP) { inf = true; break; }
            uint64_t q = A380931_P[t];
            if (prod > rem / q) break;
            prod *= q;
            f *= (double)q / (double)(q - 1);
            t++;
        }
        if (!inf && ab * f < 4.0 - 1e-9) return;
        uint64_t pe = p, sp = 1 + p;
        A380931_fp[A380931_fd] = p;
        A380931_fd++;
        for (uint64_t e = 1;; e++) {
            A380931_fe[A380931_fd - 1] = e;
            uint64_t nm = m * pe, ns = sig * sp;
            if ((unsigned __int128)ns > (unsigned __int128)nm * 4) A380931_check(nm, ns);
            A380931_dfs(i + 1, nm, ns);
            if (pe > rem / p) break;
            pe *= p;
            sp += pe;
        }
        A380931_fd--;
    }
    /* the prime table is used up: any further prime q > sqrt(N), so only k = m*q */
    uint64_t q = A380931_PLIM + 1;
    if ((q & 1) == 0) q++;
    for (; q <= rem; q += 2) {
        if (!((unsigned __int128)sig * (q + 1) > (unsigned __int128)m * q * 4)) break;
        if (is_prime_u64(q)) {
            A380931_fp[A380931_fd] = q;
            A380931_fe[A380931_fd] = 1;
            A380931_fd++;
            A380931_check(m * q, sig * (q + 1));
            A380931_fd--;
        }
    }
}

static int A380931_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static uint64_t A380931_isqrt(uint64_t x)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

seq_t a(idx_t n)
{
    while (!A380931_done || A380931_nres < (size_t)n) {
        A380931_N = A380931_N ? A380931_N * 2 : ((uint64_t)1 << 26);
        uint64_t lim = A380931_N / 1000;
        if (lim > 30000000ULL) lim = 30000000ULL;
        uint64_t sq = A380931_isqrt(A380931_N) + 2;
        if (lim < sq) lim = sq;
        if (lim < 1000) lim = 1000;
        A380931_sieve(lim);
        A380931_nres = 0;
        A380931_fd = 0;
        A380931_dfs(0, 1, 1);
        qsort(A380931_res, A380931_nres, sizeof(uint64_t), A380931_cmp);
        A380931_done = true;
    }
    return (seq_t)A380931_res[n - 1];
}
