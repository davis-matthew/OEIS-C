/*
 * A327925  Irregular table read by rows: T(m,n) is the number of
 *          non-isomorphic groups G such that G is the semidirect product of
 *          C_m and C_n, where C_m is a normal subgroup of G and C_n is a
 *          subgroup of G, 1 <= n <= A002322(m).
 * https://oeis.org/A327925
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = the n-th entry in the irregular table T(m,n) read by
 *            rows, where T(m,n) is the number of non-isomorphic groups G that
 *            are the semidirect product of a normal cyclic group of order m
 *            by a cyclic subgroup of order n. T(m,n) = Σ_{d|n} (count of
 *            elements of order d in (Z/mZ)^*)/φ(d)
 * Range:     u8 n <= 8346, u16 n <= 8346, u32 n <= 8346, u64 n <= 8346,
 *            u128 n <= 8346, big any n
 * Validated: standard, pass, 124 terms through n = 7783 (2026-11-03)
 * Helpers:   gcd_u64, mem, powmod, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mem.h"

#ifndef A327925_H
#define A327925_H

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"
#include "../utils/powmod.h"
#include "../utils/totient_seq.h"

/* ---------- divisor generation from prime factorization ---------- */
static int cmpSize(const void *a, const void *b)
{
    size_t x = *(const size_t *)a;
    size_t y = *(const size_t *)b;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

/* ---------- integer helpers ---------- */

static uint64_t lcm_uint64(uint64_t a, uint64_t b)
{
    return a / gcd_u64(a, b) * b;
}

/* ---------- factorization (trial division) ---------- */
static void factor_uint64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    for (uint64_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            pr[(*cnt)] = p;
            ex[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        pr[(*cnt)] = n;
        ex[(*cnt)] = 1;
        ++(*cnt);
    }
}

static void divisors_from_factors(const uint64_t *pr, const uint64_t *ex,
                                  size_t cnt, size_t *out, size_t *outcnt)
{
    size_t cur[256], curcnt = 1;
    cur[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i], e = ex[i];
        size_t oldcnt = curcnt;
        for (size_t j = 0; j < oldcnt; ++j) {
            uint64_t val = cur[j];
            for (size_t k = 1; k <= e; ++k) {
                val *= p;
                cur[curcnt++] = val;
            }
        }
    }
    memcpy(out, cur, curcnt * sizeof(size_t));
    qsort(out, curcnt, sizeof(size_t), cmpSize);
    *outcnt = curcnt;
}

/* ---------- Carmichael λ function ---------- */
static uint64_t carmichael_lambda(uint64_t m)
{
    if (m == 1) return 1;
    uint64_t pr[64], ex[64];
    size_t pcnt;
    factor_uint64(m, pr, ex, &pcnt);
    uint64_t lam = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        uint64_t p = pr[i], e = ex[i];
        uint64_t comp;
        if (p == 2) {
            if (e == 1) comp = 1;
            else if (e == 2) comp = 2;
            else comp = 1ULL << (e - 2);
        } else {
            uint64_t pe = 1;
            for (uint64_t k = 1; k < e; ++k) pe *= p; /* p^(e-1) */
            comp = (p - 1) * pe;
        }
        lam = lcm_uint64(lam, comp);
    }
    return lam;
}

/* ---------- cache for sequence values ---------- */
static seq_t *cache_vals = NULL;
static size_t cache_size = 0, cache_cap = 0;
static uint64_t next_m_to_generate = 1;

/* expand cache capacity */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 256;
    while (new_cap < needed) new_cap *= 2;
    cache_vals = mem_resize(seq_t, cache_vals, new_cap);
    if (!cache_vals) exit(EXIT_FAILURE);
    cache_cap = new_cap;
}

/* generate one full row for a given m and append it to cache */
static void generate_row(uint64_t m)
{
    uint64_t lam_uint = carmichael_lambda(m);
    size_t lam = (size_t)lam_uint;

    /* phi(m) */
    seq_t phi_val = totient_seq((idx_t)m);
    size_t phi = (size_t)phi_val; /* fits in size_t for our bounds */

    /* divisors of lam */
    size_t divsLambda[256], divCntLambda = 0;
    uint64_t prLam[64], exLam[64];
    size_t pcntLam;
    factor_uint64(lam_uint, prLam, exLam, &pcntLam);
    divisors_from_factors(prLam, exLam, pcntLam, divsLambda, &divCntLambda);

    /* count number of elements of each order d in (Z/mZ)^* */
    seq_t *count_order = mem_new(seq_t, phi + 1);
    if (!count_order) exit(EXIT_FAILURE);

    if (m == 1) {
        count_order[1] = 1;
    } else {
        for (uint64_t x = 1; x < m; ++x) {
            if (gcd_u64(x, m) != 1) continue;
            /* find minimal d|lam with x^d ≡ 1 mod m */
            for (size_t idx = 0; idx < divCntLambda; ++idx) {
                size_t d = divsLambda[idx];
                if (powmod((seq_t)x, d, (seq_t)m) == 1) {
                    count_order[d] += 1;
                    break;
                }
            }
        }
    }

    /* precompute contr[d] = count_order[d] / φ(d) for all d|lam */
    seq_t *contr = mem_new(seq_t, lam + 1);
    if (!contr) exit(EXIT_FAILURE);
    for (size_t i = 0; i < divCntLambda; ++i) {
        size_t d = divsLambda[i];
        if (count_order[d] == 0) continue;
        seq_t phi_d = totient_seq((idx_t)d);
        contr[d] = count_order[d] / phi_d;
    }

    /* build row via divisor convolution */
    seq_t *row = mem_new(seq_t, lam + 1);
    if (!row) exit(EXIT_FAILURE);
    for (size_t i = 0; i < divCntLambda; ++i) {
        size_t d = divsLambda[i];
        seq_t val = contr[d];
        if (val == 0) continue;
        for (size_t j = d; j <= lam; j += d)
            row[j] += val;
    }

    /* append row to cache */
    ensure_cache_cap(cache_size + lam);
    for (size_t i = 1; i <= lam; ++i)
        cache_vals[cache_size++] = row[i];

    /* clean up */
    mem_free(count_order);
    mem_free(contr);
    mem_free(row);
    next_m_to_generate++;
}

/* ---------- public sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for n=0 */
    /* generate rows until we cover index n */
    while (cache_size < (size_t)n) {
        generate_row(next_m_to_generate);
    }
    return cache_vals[(size_t)n - 1];
}

#endif /* A327925_H */
