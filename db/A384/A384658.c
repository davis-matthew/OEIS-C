/*
 * A384658  Primitive terms in A384657: numbers k such that A384655(k) > k and
 *          A384655(d) <= d for every proper divisor d of k.
 * https://oeis.org/A384658
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k with A384655(k) > k and A384655(d) <= d for all
 *            proper divisors d of k, where A384655(n) = Sum_{d|n}
 *            A051903(d)*phi(n/d); found by a sieve-style search over k, with
 *            multiples of found terms excluded
 * Range:     u8 n <= 7, u16 n <= 455, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 67 terms through n = 8950 (2026-11-10)
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"

static void A384658_rec(int idx, const uint64_t *p, const uint64_t *e, int pcnt,
                        uint64_t max_e, uint64_t phi_acc, uint64_t *acc)
{
    if (idx == pcnt) {
        *acc += max_e * phi_acc;
        return;
    }
    uint64_t prime = p[idx];
    uint64_t ex = e[idx];
    uint64_t powk[65];
    powk[0] = 1;
    for (uint64_t k = 1; k <= ex; ++k) powk[k] = powk[k - 1] * prime;
    for (uint64_t d = 0; d <= ex; ++d) {
        uint64_t rem = ex - d;
        uint64_t part = (rem == 0) ? 1 : (prime - 1) * powk[rem - 1];
        A384658_rec(idx + 1, p, e, pcnt, d > max_e ? d : max_e, phi_acc * part, acc);
    }
}

static uint64_t A384658_f(uint64_t n)
{
    uint64_t primes[20], exps[20];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);
    uint64_t sum = 0;
    A384658_rec(0, primes, exps, (int)pcnt, 0, 1, &sum);
    return sum;
}

static uint64_t *A384658_terms = NULL;
static size_t A384658_cnt = 0, A384658_cap = 0;
static unsigned char *A384658_cov = NULL;
static uint64_t A384658_limit = 0;
static uint64_t A384658_next = 2;

static void A384658_mark(uint64_t p)
{
    for (uint64_t m = 2 * p; m < A384658_limit; m += p) A384658_cov[m] = 1;
}

static void A384658_grow(void)
{
    uint64_t nl = A384658_limit ? A384658_limit * 2 : 4096;
    free(A384658_cov);
    A384658_cov = (unsigned char *)calloc((size_t)nl, 1);
    A384658_limit = nl;
    for (size_t i = 0; i < A384658_cnt; i++) A384658_mark(A384658_terms[i]);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    while (A384658_cnt < (size_t)n) {
        if (A384658_next >= A384658_limit) A384658_grow();
        uint64_t k = A384658_next++;
        if (A384658_cov[k]) continue;
        if (A384658_f(k) > k) {
            if (A384658_cnt == A384658_cap) {
                A384658_cap = A384658_cap ? A384658_cap * 2 : 256;
                A384658_terms = (uint64_t *)realloc(A384658_terms, A384658_cap * sizeof(uint64_t));
            }
            A384658_terms[A384658_cnt++] = k;
            A384658_mark(k);
        }
    }
    return (seq_t)A384658_terms[n - 1];
}
