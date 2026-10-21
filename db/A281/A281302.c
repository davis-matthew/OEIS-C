/*
 * A281302  Largest nonnegative k such that binomial(2*c-1, c-1) == 1 (mod
 *          c^k), where c is the n-th composite number.
 * https://oeis.org/A281302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 640, u16 n <= 8007, u32 n <= 8007, u64 n <= 8007,
 *            u128 n <= 8007, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A281302: a(n) = largest k ≥ 0 such that
 * binomial(2*c-1, c-1) ≡ 1 (mod c^k),
 * where c is the n‑th composite number. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

/* ---------- Helpers ---------- */

/* 64‑bit modular inverse using extended Euclidean algorithm.
 * Assumes a and mod are coprime. */
static uint64_t mod_inv_u64(uint64_t a, uint64_t mod)
{
    int64_t b = (int64_t)mod, u  = 1, v = 0;
    while (b)
    {
        int64_t t = a / b;
        a -= t * b; int64_t tmp = a; a = b; b = tmp;
        u -= t * v; tmp = u; u = v; v = tmp;
    }
    if (a != 1) return 0;          /* inverse does not exist – should not happen */
    if (u < 0) u += (int64_t)mod;
    return (uint64_t)u;
}

/* 64‑bit power with overflow detection.
 * Returns UINT64_MAX if overflow would occur. */
static uint64_t pow_uint64(uint64_t base, idx_t exp)
{
    __uint128_t res = 1;
    for (idx_t i = 0; i < exp; ++i)
    {
        res *= base;
        if (res > UINT64_MAX) return UINT64_MAX;
    }
    return (uint64_t)res;
}

/* Computes binomial(N,K) mod mod where mod is a prime power p^t.
 * The algorithm removes factors of p from numerators and denominators
 * before taking the modular inverse. Works for any 64‑bit modulus. */
static uint64_t binom_mod_prime_power_64(idx_t N, idx_t K, idx_t p, uint64_t mod)
{
    uint64_t res = 1;
    int64_t exp = 0;            /* exponent of p in binomial */
    for (idx_t i = 1; i <= K; ++i)
    {
        idx_t num = N - K + i;
        idx_t den = i;

        int ns = 0;
        while ((num % p) == 0) { num /= p; ++ns; }

        int ds = 0;
        while ((den % p) == 0) { den /= p; ++ds; }

        exp += ns - ds;

        uint64_t num_mod = (uint64_t)(num % mod);
        res = (__uint128_t)res * num_mod % mod;

        uint64_t den_mod = (uint64_t)(den % mod);
        uint64_t inv = mod_inv_u64(den_mod, mod);
        res = (__uint128_t)res * inv % mod;
    }

    /* multiply back p^exp modulo mod */
    for (int64_t i = 0; i < exp && i < 64; ++i)
    {
        res = (__uint128_t)res * p % mod;
        if (res == 0) break;
    }
    return res;
}

/* ---------- Composite storage ---------- */

/* Dynamic array of composites. */
static seq_t *composites = NULL;
static size_t composites_cnt = 0;
static size_t composites_cap = 0;

static void ensure_composites_cap(size_t needed)
{
    if (composites_cap >= needed) return;
    size_t new_cap = composites_cap ? composites_cap * 2 : 32;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(composites, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    composites = tmp;
    composites_cap = new_cap;
}

static void ensure_composites(idx_t needed)
{
    if (composites_cnt >= needed) return;
    ensure_composites_cap(needed);

    idx_t start = 4;
    if (composites_cnt > 0)
        start = (idx_t)(composites[composites_cnt - 1]) + 1;

    idx_t x = start;
    while (composites_cnt < needed)
    {
        if (!is_prime(x))
            composites[composites_cnt++] = (seq_t)x;
        ++x;
    }
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    ensure_composites(n);
    idx_t c = (idx_t)composites[n - 1];

    /* Factor c */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pf_cnt = 0;
    factor_u64((uint64_t)c, primes, exps, &pf_cnt);

    unsigned best = (unsigned)-1;   /* will become min over primes */

    for (size_t i = 0; i < pf_cnt; ++i)
    {
        idx_t p = (idx_t)primes[i];
        unsigned e_c = (unsigned)exps[i];

        /* Find v_p(binom(2c-1,c-1)-1) */
        unsigned v = 0;
        unsigned t = 1;
        while (1)
        {
            uint64_t mod = pow_uint64(p, t);
            if (mod == UINT64_MAX) break;   /* overflow – higher powers impossible anyway */

            uint64_t bin_mod = binom_mod_prime_power_64(2 * c - 1, c - 1, p, mod);
            uint64_t diff = (bin_mod + mod - 1) % mod;   /* bin(Mod) - 1 mod mod */

            if (diff == 0)
            {
                ++v;          /* p^t divides diff */
                ++t;
            }
            else
                break;
        }

        unsigned k_p = v / e_c;                /* floor(v / e_c) */
        if (k_p < best) best = k_p;
        if (best == 0) break;                  /* cannot get smaller */
    }

    if (best == (unsigned)-1) best = 0;
    return (seq_t)best;
}
