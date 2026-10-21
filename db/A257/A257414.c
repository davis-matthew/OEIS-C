/*
 * A257414  Values of n such that there are exactly 7 solutions to x^2 - y^2 =
 *          n with x > y >= 0.
 * https://oeis.org/A257414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compare_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A257414.  The term a(n) is the n‑th integer n such that the
   Diophantine equation x² – y² = n has exactly seven solutions with
   x > y ≥ 0.  For an odd integer m the number of solutions is
   ceil(τ(m)/2) and for an integer divisible by 4 it is
   ceil(τ(m/4)/2).  Consequently a(n) corresponds to integers
   m with τ(m) = 13 or 14; each such m contributes the odd value n = m
   (if m is odd) and the even value n = 4·m.  This implementation
   generates all suitable m up to a fixed bound, enumerates the
   factorisations p¹², p¹³, and p⁶·q (p ≠ q), collects the
   corresponding n‑values, sorts and deduplicates them, and returns
   the requested element.  The list is exhaustive for the required
   number of terms (≈ 10 000) with the chosen upper bound.  */
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/compare_seq_t.h"

/* multiply base^exp with early termination if product would exceed
   limit; used only for 32‑bit integer types.  */
static uint32_t pow_limit(uint32_t base, unsigned int exp, uint32_t limit)
{
    uint64_t res = 1;
    for (unsigned int i = 0; i < exp; ++i) {
        if (res > limit / base) return limit + 1U;
        res *= base;
    }
    return (uint32_t)res;
}

/* static storage for the sequence */
static seq_t      *seq = NULL;
static size_t     seq_len = 0;
static int        initialized = 0;

/* fill seq with all terms up to a given bound on m; the bound must
   be large enough to produce at least the required number of terms. */
static void generate_sequence(void)
{
    if (initialized) return;
    initialized = 1;

    /* upper bound for m (≈ 20 million is enough for ~10 000 terms) */
    const uint32_t LIM_M = 20000000U;

    /* ---------- sieve primes up to LIM_M ---------- */
    unsigned char *is_prime = (unsigned char *)malloc(LIM_M + 1);
    if (!is_prime) exit(EXIT_FAILURE);
    memset(is_prime, 1, LIM_M + 1);
    is_prime[0] = is_prime[1] = 0;
    for (uint32_t i = 2; (uint64_t)i * i <= LIM_M; ++i) {
        if (i && is_prime[i]) {
            for (uint32_t j = i * i; j <= LIM_M; j += i)
                is_prime[j] = 0;
        }
    }

    /* count primes */
    size_t primes_cnt = 0;
    for (uint32_t i = 2; i <= LIM_M; ++i)
        if (is_prime[i]) ++primes_cnt;

    uint32_t *primes = (uint32_t *)malloc(primes_cnt * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);
    size_t pidx = 0;
    for (uint32_t i = 2; i <= LIM_M; ++i)
        if (is_prime[i]) primes[pidx++] = i;
    free(is_prime);  /* primes array is built; sieve no longer needed */

    /* ---------- generate candidate n values ---------- */
    size_t cap = 100000;
    seq_t *cand = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cand) exit(EXIT_FAILURE);
    size_t len = 0;
#define ADD_CAND(v) do { \
    if (len == cap) { \
        cap <<= 1; \
        cand = (seq_t *)realloc(cand, cap * sizeof(seq_t)); \
        if (!cand) exit(EXIT_FAILURE); \
    } \
    cand[len++] = (v); \
} while (0)

    /* 1. p^12 and p^13 */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint32_t m12 = pow_limit(p, 12U, LIM_M);
        if (m12 <= LIM_M) {
            if (m12 & 1U) ADD_CAND((seq_t)m12);          /* odd m */
            ADD_CAND((seq_t)((uint64_t)m12 * 4ULL));      /* 4*m case */
        }
        uint32_t m13 = pow_limit(p, 13U, LIM_M);
        if (m13 <= LIM_M) {
            if (m13 & 1U) ADD_CAND((seq_t)m13);
            ADD_CAND((seq_t)((uint64_t)m13 * 4ULL));
        }
    }

    /* 2. p^6 * q with p != q */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint32_t p6 = pow_limit(p, 6U, LIM_M);
        if (p6 > LIM_M) continue;
        uint32_t max_q = LIM_M / p6;
        for (size_t j = 0; j < primes_cnt; ++j) {
            uint32_t q = primes[j];
            if (q == p) continue;
            if (q > max_q) break;
            uint32_t m = p6 * q;
            if (m > LIM_M) break;
            if (m & 1U) ADD_CAND((seq_t)m);
            ADD_CAND((seq_t)((uint64_t)m * 4ULL));
        }
    }

    free(primes);

    /* ---------- sort and deduplicate ---------- */
    qsort(cand, len, sizeof(seq_t), compare_seq_t);
    size_t uniq = 0;
    for (size_t i = 0; i < len; ++i) {
        if (uniq == 0 || compare_seq_t(&cand[uniq - 1], &cand[i]) != 0) {
            cand[uniq++] = cand[i];
        }
    }
    seq = cand;
    seq_len = uniq;
}

/* ------------------------------------------------------------------ */
/* public function that returns the n‑th term of the sequence          */
seq_t a(idx_t n)
{
    generate_sequence();
    if (n == 0 || n > seq_len) return (seq_t)0;  /* undefined range */
    return seq[n - 1];
}
