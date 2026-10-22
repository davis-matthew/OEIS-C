/*
 * A328926  Numbers k such that A328925(k) = 1; numbers k such that if we
 *          write k = Product_{i=1..t} p_i^e_i, then lcm_{1<=i,j<=t,i!=j}
 *          ord(p_i,p_j^e_j) = A002322(k), where ord(a,r) is the
 *          multiplicative order of a modulo r, and A002322 is the Carmichael
 *          lambda (usually written as psi).
 * https://oeis.org/A328926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes_upto, gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A328926: Numbers k such that A328925(k) = 1.
   Computation: a(n) is the nth integer k for which
   λ(k) = lcm_{i≠j} ord(p_i, p_j^{e_j}),
   where k = ∏ p_i^{e_i} and λ is Carmichael's lambda.  */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mod_pow.h"
#include "../utils/gcd.h"   /* provides static seq_t gcd(seq_t a, seq_t b) */

/* ---------- prime list ---------- */
static bool primes_inited = false;
static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;

static void init_primes(void)
{
    if (primes_inited) return;
    /* 2 000 000 is comfortably larger than sqrt of the 10 000‑th term. */
    build_primes_upto(2000000, &prime_list, &prime_cnt);
    primes_inited = true;
}

/* ---------- utility functions ---------- */
static inline uint64_t pow_uint64(uint64_t base, uint64_t exp)
{
    uint64_t r = 1;
    while (exp--) r *= base;
    return r;
}

static inline uint64_t lcm64(uint64_t a, uint64_t b)
{
    return a / (uint64_t)gcd((seq_t)a, (seq_t)b) * b;
}

/* λ(p^e) for a prime power */
static uint64_t lambda_prime_power(uint64_t p, uint64_t e)
{
    if (p == 2) {
        if (e == 1) return 1;
        if (e == 2) return 2;
        return 1ULL << (e - 2);   /* 2^(e-2) for e≥3 */
    }
    return (p - 1ULL) * pow_uint64(p, e - 1);
}

/* factorization of n using the global prime list (uint64_t only) */
static void factor_uint64(uint64_t n, uint64_t *pf, int *exp, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) {
            int e = 0;
            while (n % prime_list[i] == 0) {
                n /= prime_list[i];
                ++e;
            }
            pf[(*cnt)] = prime_list[i];
            exp[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        pf[(*cnt)] = n;
        exp[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* multiplicative order of a modulo p^e, where p is prime.  The
   order divides λ(p^e). */
static uint64_t multiplicative_order(uint64_t a, uint64_t p, uint64_t e)
{
    uint64_t mod = pow_uint64(p, e);
    uint64_t order = lambda_prime_power(p, e);

    /* factor λ(p^e) */
    uint64_t fac[64];
    int    exp_fac[64];
    size_t cnt_fac;
    factor_uint64(order, fac, exp_fac, &cnt_fac);

    /* reduce order by each prime factor as long as possible */
    for (size_t i = 0; i < cnt_fac; ++i) {
        uint64_t q = fac[i];
        while (order % q == 0) {
            uint64_t cand = order / q;
            if (mod_pow((seq_t)a, (idx_t)cand, (seq_t)mod) == 1)
                order = cand;
            else
                break;
        }
    }
    return order;
}

/* does k satisfy the definition? k=1 and 2 are special cases. */
static bool is_valid_k(uint64_t k)
{
    if (k == 1 || k == 2) return true;

    uint64_t pk[64];
    int    ep[64];
    size_t cntk;

    factor_uint64(k, pk, ep, &cntk);
    if (cntk == 1) return false;   /* single prime power other than 2 fails */

    /* λ(k) */
    uint64_t lambda_k = 1;
    for (size_t i = 0; i < cntk; ++i)
        lambda_k = lcm64(lambda_k, lambda_prime_power(pk[i], ep[i]));

    /* lcm of all pairwise orders */
    uint64_t lcm_orders = 1;
    for (size_t i = 0; i < cntk; ++i) {
        for (size_t j = 0; j < cntk; ++j) if (i != j) {
            uint64_t order = multiplicative_order(pk[i], pk[j], ep[j]);
            lcm_orders = lcm64(lcm_orders, order);
        }
    }

    return lcm_orders == lambda_k;
}

/* ---------- cache for sequence values ---------- */
static seq_t *cache = NULL;          /* array of found terms */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cache_cnt = 0;         /* number of terms stored */
static uint64_t next_candidate = 1;   /* next integer to test */

static void append_cache(seq_t val)
{
    if (cache_cnt == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_cnt++] = val;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;
    init_primes();

    /* fill cache up to n */
    while (cache_cnt < (size_t)n) {
        if (is_valid_k(next_candidate))
            append_cache((seq_t)next_candidate);
        ++next_candidate;
    }
    return cache[(size_t)n - 1];
}
