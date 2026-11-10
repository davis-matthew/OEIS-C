/*
 * A372571  Lexicographically earliest infinite sequence such that a(i) = a(j)
 *          => A324198(i) = A324198(j) and A342671(i) = A342671(j), for all i,
 *          j >= 1.
 * https://oeis.org/A372571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = RGS transform of the pair [A324198(n), A342671(n)]: a(n)
 *            = index (from 1) of the first k with the same pair, numbered in
 *            order of first appearance
 * Range:     u8 n <= 53, u16 n <= 4351, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 120 terms through n = 9854 (2026-11-10)
 * Helpers:   build_primes_upto, factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

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
#define A342671_LIMIT 200000U
#define A342671_MAXP   18000U

static uint32_t A342671_primes[A342671_MAXP];
static size_t   A342671_pcnt = 0;
static bool     A342671_inited = false;

static void A342671_init(void)
{
    static bool mark[A342671_LIMIT + 1];
    if (A342671_inited) return;
    for (uint32_t i = 2; i <= A342671_LIMIT; ++i) {
        if (!mark[i]) {
            if (A342671_pcnt < A342671_MAXP) A342671_primes[A342671_pcnt++] = i;
            for (uint64_t j = (uint64_t)i * i; j <= A342671_LIMIT; j += i)
                mark[(size_t)j] = true;
        }
    }
    A342671_inited = true;
}

static uint32_t A342671_next_prime(uint32_t p)
{
    size_t lo = 0, hi = A342671_pcnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (A342671_primes[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    if (lo + 1 < A342671_pcnt) return A342671_primes[lo + 1];
    uint32_t q = p + 1;
    while (true) {
        bool ip = true;
        for (size_t k = 0; k < A342671_pcnt && (uint64_t)A342671_primes[k] * A342671_primes[k] <= q; ++k) {
            if (q % A342671_primes[k] == 0) { ip = false; break; }
        }
        if (ip) return q;
        ++q;
    }
}

static seq_t A342671_gcd(seq_t a, seq_t b)
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
    A342671_init();

    seq_t sg = (seq_t)1;
    seq_t a3961 = (seq_t)1;

    idx_t m = n;
    for (size_t i = 0; i < A342671_pcnt && (uint64_t)A342671_primes[i] * A342671_primes[i] <= (uint64_t)m; ++i) {
        uint32_t p = A342671_primes[i];
        if (m % p == 0) {
            uint32_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t p_pow = (seq_t)1;
            for (uint32_t k = 0; k < e; ++k) p_pow *= (seq_t)p;
            seq_t p_pow1 = p_pow * (seq_t)p;
            sg *= (p_pow1 - (seq_t)1) / ((seq_t)p - (seq_t)1);

            uint32_t q = A342671_next_prime(p);
            seq_t q_pow = (seq_t)1;
            for (uint32_t k = 0; k < e; ++k) q_pow *= (seq_t)q;
            a3961 *= q_pow;
        }
    }

    if (m > 1) {
        uint32_t p = (uint32_t)m;
        sg *= (seq_t)p + (seq_t)1;
        uint32_t q = A342671_next_prime(p);
        a3961 *= (seq_t)q;
    }

    return A342671_gcd(sg, a3961);
}

/* ---------------- RGS transform ---------------- */
static seq_t   *K1 = NULL;
static seq_t   *K2 = NULL;
static size_t   Dcnt = 0, Dcap = 0;
static idx_t   *Rgs = NULL;
static idx_t    Done = 0, RgsCap = 0;

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n > RgsCap) {
        idx_t nc = (n < 1024) ? 1024 : n + n / 2;
        idx_t *nr = (idx_t *)realloc(Rgs, (size_t)(nc + 1) * sizeof(idx_t));
        if (!nr) return (seq_t)0;
        Rgs = nr;
        RgsCap = nc;
    }
    while (Done < n) {
        idx_t m = Done + 1;
        seq_t x = A324198_a(m);
        seq_t y = A342671_a(m);
        size_t j = 0;
        for (; j < Dcnt; ++j) {
            if (K1[j] == x && K2[j] == y) break;
        }
        if (j == Dcnt) {
            if (Dcnt == Dcap) {
                size_t nc = Dcap ? Dcap * 2 : 256;
                seq_t *n1 = mem_new(seq_t, nc);
                seq_t *n2 = mem_new(seq_t, nc);
                for (size_t t = 0; t < Dcnt; ++t) { n1[t] = K1[t]; n2[t] = K2[t]; }
                if (K1) mem_free(K1);
                if (K2) mem_free(K2);
                K1 = n1; K2 = n2; Dcap = nc;
            }
            K1[Dcnt] = x;
            K2[Dcnt] = y;
            Dcnt++;
        }
        Rgs[m] = (idx_t)(j + 1);
        Done = m;
    }
    return (seq_t)Rgs[n];
}
