/*
 * A329062  Numbers k that are not prime powers (i.e., not in A000961) such
 *          that A328925(k) > 1; numbers k such that if we write k =
 *          Product_{i=1..t} p_i^e_i , then t > 1, and lcm_{1<=i,j<=t, i!=j}
 *          ord(p_i,p_j^e_j) < A002322(k), where ord(a,r) is the
 *          multiplicative order of a modulo r, and A002322 is the Car...
 * https://oeis.org/A329062
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth integer k that is NOT a prime power and satisfies
 *            * b(k) < ψ(k), where ψ is Carmichael λ and b is the lcm of the
 *            multiplicative * orders of each prime factor modulo the
 *            prime‑power factors of the other * distinct primes in k
 * Range:     u8 n <= 45, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   factor_u64, gcd, mod_pow, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* utility headers supplied by the build system */
#include "../utils/factor_u64.h"      /* void factor_u64(uint64_t n, uint64_t *pr,
                                       uint64_t *ex, size_t *cnt);          */
#include "../utils/mod_pow.h"        /* seq_t mod_pow(seq_t base, uint64_t exp,
                                       seq_t mod);                              */
#include "../utils/gcd.h"            /* seq_t gcd(seq_t a, seq_t b);             */
#include "../utils/seq_alloc.h"      /* seq_t *seq_alloc(size_t cap);           */
#include "../utils/seq_realloc.h"    /* seq_t *seq_realloc(seq_t *old, size_t
                                       old_cap, size_t new_cap);              */

/* ----------------------------------------------------------------- */
/* Helper: Carmichael λ for a prime power p^e */
static inline seq_t lambda_prime_power(uint64_t p, uint64_t e)
{
    if (p == 2) {
        if (e == 1) return (seq_t)1;
        if (e == 2) return (seq_t)2;
        return (seq_t)1 << (e - 2);          /* 2^(e-2) */
    }
    /* λ(p^e) = p^(e-1) * (p-1) for odd p */
    seq_t res = (seq_t)(p - 1);
    for (uint64_t i = 1; i < e; ++i) res *= (seq_t)p;
    return res;
}

/* ----------------------------------------------------------------- */
/* Helper: lcm for seq_t using 128‑bit intermediate arithmetic */
static inline seq_t lcm_seq_t(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    if (g == 0) return 0;
    return (seq_t)(((__int128)(a / g)) * b);
}

/* ----------------------------------------------------------------- */
/* Helper: multiplicative order of a modulo mod (gcd(a,mod)=1) */
static seq_t order_mod(uint64_t a, uint64_t mod)
{
    /* factor mod to get its single prime and exponent */
    uint64_t pf[1], ef[1];
    size_t cnt = 0;
    factor_u64(mod, pf, ef, &cnt);            /* cnt must be 1 */
    uint64_t p = pf[0], e = ef[0];

    seq_t lam = lambda_prime_power(p, e);      /* λ(mod) */

    /* factor λ(mod) to test divisors */
    uint64_t fac[64], exp[64];
    size_t faccnt = 0;
    factor_u64((uint64_t)lam, fac, exp, &faccnt);

    seq_t ord = lam;
    for (size_t i = 0; i < faccnt; ++i) {
        uint64_t f = fac[i];
        while (ord % f == 0) {
            seq_t test = mod_pow(a, (uint64_t)(ord / f), mod);
            if (test == (seq_t)1) ord /= f;
            else break;
        }
    }
    return ord;
}

/* ----------------------------------------------------------------- */
/* Global state for computed terms (dynamic cache) */
static seq_t *term_cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t next_k = 2;    /* start scanning from 2 */

/* ----------------------------------------------------------------- */
/* Ensure at least n terms are present in the cache */
static void ensure_cache(size_t n)
{
    while (cache_len < n) {
        uint64_t k = next_k++;
        /* factor k */
        uint64_t pr[64], ex[64];
        size_t cnt = 0;
        factor_u64(k, pr, ex, &cnt);
        if (cnt <= 1) continue;   /* skip prime powers */

        /* compute ψ(k) = lcm of λ(p^e) over prime powers */
        seq_t psi = 1;
        for (size_t i = 0; i < cnt; ++i) {
            seq_t lam = lambda_prime_power(pr[i], ex[i]);
            psi = lcm_seq_t(psi, lam);
        }

        /* compute b(k) = lcm of ord(p_i, p_j^e_j) over i≠j */
        seq_t b = 1;
        /* pre‑compute modulus values p_j^e_j */
        uint64_t mods[64];
        for (size_t j = 0; j < cnt; ++j) {
            uint64_t m = 1;
            for (uint64_t t = 0; t < ex[j]; ++t) m *= pr[j];
            mods[j] = m;
        }
        for (size_t i = 0; i < cnt; ++i) {
            for (size_t j = 0; j < cnt; ++j) {
                if (i == j) continue;
                seq_t ord = order_mod(pr[i], mods[j]);
                b = lcm_seq_t(b, ord);
            }
        }

        if (b < psi) {
            /* enlarge cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = (cache_cap ? cache_cap * 2 : 8);
                term_cache = seq_realloc(term_cache, cache_cap, new_cap);
                cache_cap = new_cap;
            }
            term_cache[cache_len++] = (seq_t)k;
        }
    }
}

/* ----------------------------------------------------------------- */
/* Public function required by the judge */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for 0; not used in tests */
    ensure_cache((size_t)n);
    return term_cache[n - 1];
}
