/*
 * A355002  Numbers k that share a prime factor with A355456(k).
 * https://oeis.org/A355002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n-th k >= 1 with gcd(A324198(k), A342671(k)) > 1;
 *            computed by a memoized search over k
 * Range:     u8 none, u16 n <= 52, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-10)
 * Helpers:   build_primes_upto, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* ---------------- A324198 ---------------- */
static seq_t A324198_a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    uint64_t *primes = NULL;
    size_t prime_cnt = 0;
    build_primes_upto(n, &primes, &prime_cnt);

    uint64_t weight[64];
    weight[0] = 1;
    size_t k = 0;
    while (k < prime_cnt && weight[k] <= n / primes[k]) {
        weight[k + 1] = weight[k] * primes[k];
        k++;
    }
    size_t max_k = k;

    uint64_t digits[64] = {0};
    uint64_t rem = n;
    for (int i = (int)max_k; i >= 0; --i) {
        digits[i] = rem / weight[i];
        rem     %= weight[i];
    }

    uint64_t fp[20], fe[20];
    size_t fc = 0;
    factor_u64(n, fp, fe, &fc);

    seq_t result = (seq_t)1;
    for (size_t idx = 0; idx < fc; ++idx) {
        uint64_t p   = fp[idx];
        uint64_t eN  = fe[idx];

        size_t j = 0;
        for (; j < prime_cnt; ++j) {
            if (primes[j] == p) break;
        }
        if (j >= prime_cnt) continue;

        uint64_t eA = (j <= max_k) ? digits[j] : (uint64_t)0;
        uint64_t e  = (eN < eA) ? eN : eA;

        for (uint64_t t = 0; t < e; ++t) {
            result *= (seq_t)p;
        }
    }

    free(primes);
    return result;
}

/* ---------------- A342671 ---------------- */
#define A342671_PRIME_TABLE_LIMIT 200000U
#define A342671_MAX_PRIMES          18000U

static uint32_t A342671_primes[A342671_MAX_PRIMES];
static size_t   A342671_prime_cnt = 0;
static bool     A342671_primes_inited = false;

static void A342671_init_prime_table(void)
{
    if (A342671_primes_inited) return;
    static bool mark[A342671_PRIME_TABLE_LIMIT + 1];
    for (uint32_t i = 2; i <= A342671_PRIME_TABLE_LIMIT; ++i) {
        if (!mark[i]) {
            if (A342671_prime_cnt < A342671_MAX_PRIMES) A342671_primes[A342671_prime_cnt++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A342671_PRIME_TABLE_LIMIT; j += i)
                mark[(size_t)j] = true;
        }
    }
    A342671_primes_inited = true;
}

static uint32_t A342671_next_prime_of(uint32_t p)
{
    size_t lo = 0, hi = A342671_prime_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (A342671_primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    if (lo + 1 < A342671_prime_cnt) return A342671_primes[lo + 1];
    uint32_t q = p + 1;
    while (true) {
        bool isp = true;
        for (size_t k = 0; k < A342671_prime_cnt && (uint64_t)A342671_primes[k] * A342671_primes[k] <= q; ++k) {
            if (q % A342671_primes[k] == 0) { isp = false; break; }
        }
        if (isp) return q;
        ++q;
    }
}

static seq_t A342671_seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

static seq_t A342671_a(idx_t n)
{
    A342671_init_prime_table();

    seq_t sigma_v   = (seq_t)1;
    seq_t a003961   = (seq_t)1;

    idx_t m = n;
    for (size_t i = 0; i < A342671_prime_cnt && (uint64_t)A342671_primes[i] * A342671_primes[i] <= (uint64_t)m; ++i) {
        uint32_t p = A342671_primes[i];
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t p_pow = (seq_t)1;
            for (uint32_t k = 0; k < e; ++k) p_pow *= (seq_t)p;
            seq_t p_pow_plus1 = p_pow * (seq_t)p;
            seq_t sigma_part = (p_pow_plus1 - (seq_t)1) / ((seq_t)p - (seq_t)1);
            sigma_v *= sigma_part;

            uint32_t q = A342671_next_prime_of(p);
            seq_t q_pow = (seq_t)1;
            for (uint32_t k = 0; k < e; ++k) q_pow *= (seq_t)q;
            a003961 *= q_pow;
        }
    }

    if (m > 1) {
        uint32_t p = (uint32_t)m;
        seq_t sigma_part = (seq_t)p + (seq_t)1;
        sigma_v *= sigma_part;

        uint32_t q = A342671_next_prime_of(p);
        a003961 *= (seq_t)q;
    }

    return A342671_seq_gcd(sigma_v, a003961);
}

/* ---------------- main ---------------- */
static idx_t *A355002_terms = NULL;
static size_t A355002_cnt = 0;
static size_t A355002_cap = 0;
static idx_t  A355002_next = 1;

seq_t a(idx_t n)
{
    while (A355002_cnt < (size_t)n) {
        idx_t k = A355002_next++;
        seq_t g1 = A324198_a(k);
        seq_t g2 = A342671_a(k);
        if (A342671_seq_gcd(g1, g2) > (seq_t)1) {
            if (A355002_cnt == A355002_cap) {
                size_t nc = A355002_cap ? A355002_cap * 2 : 64;
                A355002_terms = (idx_t *)realloc(A355002_terms, nc * sizeof(idx_t));
                A355002_cap = nc;
            }
            A355002_terms[A355002_cnt++] = k;
        }
    }
    return (seq_t)A355002_terms[(size_t)n - 1];
}
