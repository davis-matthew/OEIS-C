/*
 * A380875  Indices of triangular numbers (A000217) which are also perimeters
 *          of integer-sided right triangles (A010814).
 * https://oeis.org/A380875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = indices of triangular numbers that are perimeters of
 *            integer right triangles
 * Range:     u8 n <= 3, u16 n <= 146, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/u64_gcd.h"

/* Small prime table for factorization */
#define PRIME_LIMIT 50000
static uint32_t primes[5200];
static size_t prime_cnt = 0;
static int primes_inited = 0;

static void init_primes(void)
{
    if (primes_inited) return;
    for (uint32_t p = 2; p <= PRIME_LIMIT; ++p) {
        int prime = 1;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint32_t q = primes[i];
            if ((uint64_t)q * q > p) break;
            if (p % q == 0) { prime = 0; break; }
        }
        if (prime) primes[prime_cnt++] = p;
    }
    primes_inited = 1;
}

/* Factorisation of an uint64_t using the precomputed primes */
static size_t factor_uint64(uint64_t n, uint32_t *factors, uint32_t *exps)
{
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            uint32_t e = 0;
            while (n % p == 0) { n /= p; ++e; }
            factors[cnt] = p;
            exps[cnt] = e;
            ++cnt;
        }
    }
    if (n > 1) {
        factors[cnt] = (uint32_t)n;
        exps[cnt] = 1;
        ++cnt;
    }
    return cnt;
}

/* Recursively generate all divisors from factorisation */
static void gen_divisors_rec(const uint32_t *f, const uint32_t *e, size_t nf,
                             size_t idx, uint64_t cur,
                             uint64_t *out, size_t *outcnt)
{
    if (idx == nf) {
        out[(*outcnt)++] = cur;
        return;
    }
    uint64_t mult = 1;
    for (uint32_t k = 0; k <= e[idx]; ++k) {
        gen_divisors_rec(f, e, nf, idx + 1, cur * mult, out, outcnt);
        mult *= (uint64_t)f[idx];
    }
}

/* Test if p is the perimeter of an integer right triangle */
static int is_perimeter(seq_t p)
{
    if (p < 12) return 0;       /* smallest perimeter is 12 */
    if ((p & 1) != 0) return 0; /* perimeters are even */

    uint64_t X = (uint64_t)p / 2;   /* p = 2 * X */

    uint32_t fac[32], exps[32];
    size_t fcnt = factor_uint64(X, fac, exps);

    uint64_t m_divs[2000];
    size_t mcnt = 0;
    gen_divisors_rec(fac, exps, fcnt, 0, 1, m_divs, &mcnt);

    for (size_t i = 0; i < mcnt; ++i) {
        uint64_t m = m_divs[i];
        uint64_t y = X / m;

        /* factor y */
        uint32_t fac2[32], exps2[32];
        size_t fcnt2 = factor_uint64(y, fac2, exps2);

        uint64_t t_divs[2000];
        size_t tcnt = 0;
        gen_divisors_rec(fac2, exps2, fcnt2, 0, 1, t_divs, &tcnt);

        for (size_t j = 0; j < tcnt; ++j) {
            uint64_t t = t_divs[j];
            if (t <= m || t >= 2 * m) continue;
            uint64_t n_val = t - m;
            if (n_val >= m || n_val == 0) continue;
            if (u64_gcd((uint64_t)m, n_val) != 1) continue;
            if (((m & 1) == (n_val & 1))) continue;
            return 1;   /* found a valid (m,n,k) triple */
        }
    }
    return 0;
}

/* Sequence function */
seq_t a(idx_t n)
{
    /* Static cache of found indices */
    static seq_t *cache = NULL;
    static size_t cached = 0;
    static size_t cap = 0;
    static idx_t next = 1;

    if (!prime_cnt) init_primes();

    while (cached < (size_t)n) {
        seq_t tri = (seq_t)next * ((seq_t)next + 1) / 2;   /* T_next */
        if (is_perimeter(tri)) {
            if (cached == cap) {
                size_t new_cap = (cap ? cap * 2 : 4);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cap = new_cap;
            }
            cache[cached++] = (seq_t)next;
        }
        ++next;
    }
    return cache[n - 1];
}
