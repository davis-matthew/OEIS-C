/*
 * A343242  Nonprime numbers k such that A054008(k) = A054024(k).
 * https://oeis.org/A343242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(1) = 1, and the other terms are the
 *            nonprime k > 1 with sigma(k) mod k = k mod tau(k), in increasing
 *            order. Write k = A*p where p = P(k) is the largest prime factor
 *            of k. Case p^2 | k: k = A*P(A), checked directly while A is
 *            enumerated. Case p || k (p > P(A)): sigma(k...
 * Range:     u8 n <= 3, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Validated: partial, pass, 34 terms through n = 34 (2026-11-10)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

static uint64_t A343242_N = 0;
static uint32_t *A343242_pr = NULL;
static size_t A343242_pc = 0;
static uint64_t *A343242_t = NULL;
static size_t A343242_tc = 0, A343242_tcap = 0;

static void A343242_push(uint64_t k)
{
    if (A343242_tc == A343242_tcap) {
        A343242_tcap = A343242_tcap ? A343242_tcap * 2 : 64;
        A343242_t = (uint64_t *)realloc(A343242_t, A343242_tcap * sizeof(uint64_t));
    }
    A343242_t[A343242_tc++] = k;
}

static void A343242_sieve(uint64_t lim)
{
    uint8_t *c = (uint8_t *)calloc(lim + 1, 1);
    free(A343242_pr);
    A343242_pr = (uint32_t *)malloc(sizeof(uint32_t) * (lim / 2 + 16));
    A343242_pc = 0;
    for (uint64_t i = 2; i <= lim; i++) {
        if (!c[i]) {
            A343242_pr[A343242_pc++] = (uint32_t)i;
            for (uint64_t j = i * i; j <= lim; j += i) c[j] = 1;
        }
    }
    free(c);
}

/* Node A = B * q^e, q = P(A) = primes[qi]; sB = sigma(B), tB = tau(B),
   Se = 1 + q + ... + q^e, qpow = q^e.  Invariant: A*q <= N. */
static void A343242_dfs(uint64_t A, uint64_t sB, uint64_t tB, size_t qi,
                        uint64_t e, uint64_t Se, uint64_t qpow)
{
    const uint64_t N = A343242_N;
    const uint64_t q = A343242_pr[qi];

    /* Case II: k = A*q, largest prime appears squared */
    {
        uint64_t k = A * q;
        uint64_t sk = sB * (Se + qpow * q);
        uint64_t tk = tB * (e + 2);
        if (sk % k == k % tk) A343242_push(k);
    }

    /* Case I: k = A*p with prime p > q */
    {
        uint64_t s = sB * Se, tau = tB * (e + 1), T2 = 2 * tau;
        uint64_t pmax = N / A;
        if (pmax > q) {
            uint64_t jlim = s / (q + 1);
            for (uint64_t m = s / A + 1;; m++) {
                uint64_t j = m * A - s;
                if (j > jlim) break;
                for (uint64_t r = s % j; r < T2; r += j) {
                    if (r >= s) break;
                    uint64_t p = (s - r) / j;
                    if (p <= q) break;
                    if (p > pmax) continue;
                    if ((A * p) % T2 != r) continue;
                    if (!is_prime_u64(p)) continue;
                    A343242_push(A * p);
                }
            }
        }
    }

    /* children */
    if ((unsigned __int128)A * q * q <= N)
        A343242_dfs(A * q, sB, tB, qi, e + 1, Se + qpow * q, qpow * q);
    {
        uint64_t nsB = sB * Se, ntB = tB * (e + 1);
        for (size_t i = qi + 1; i < A343242_pc; i++) {
            uint64_t p = A343242_pr[i];
            if ((unsigned __int128)A * p * p > N) break;
            A343242_dfs(A * p, nsB, ntB, i, 1, 1 + p, p);
        }
    }
}

static int A343242_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A343242_compute(uint64_t N)
{
    A343242_N = N;
    A343242_tc = 0;
    A343242_push(1);
    uint64_t r = 1;
    while ((r + 1) * (r + 1) <= N) r++;
    A343242_sieve(r + 1);
    for (size_t i = 0; i < A343242_pc; i++) {
        uint64_t p = A343242_pr[i];
        if (p * p > N) break;
        A343242_dfs(p, 1, 1, i, 1, 1 + p, p);
    }
    qsort(A343242_t, A343242_tc, sizeof(uint64_t), A343242_cmp);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    while (A343242_N == 0 || A343242_tc < n) {
        uint64_t N = A343242_N ? A343242_N * 16 : ((uint64_t)1 << 16);
        A343242_compute(N);
    }
    return (seq_t)A343242_t[n - 1];
}
