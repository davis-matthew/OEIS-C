/*
 * A270839  Integers k such that (A003266(k)/A000045(k-1)) is not divisible by
 *          k.
 * https://oeis.org/A270839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer k ≥ 2 such that * (Π_{i=1}^{k} F(i)) /
 *            F(k-1) is not divisible by k, * where F(i) is the i‑th Fibonacci
 *            number (F(1)=1, F(2)=1)
 * Range:     u8 n <= 26, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   sort_uint32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Helper routines – all use only integer arithmetic. */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/sort_uint32.h"

/* --------------------- modular arithmetic --------------------- */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod)
{
    return (__uint128_t)a * b % mod;
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp) {
        if (exp & 1) res = mod_mul(res, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/* --------------------- Fibonacci (fast doubling) --------------------- */
static inline void fib_pair(uint32_t n, uint32_t mod, uint32_t *f, uint32_t *f1)
{
    if (n == 0) { *f = 0; *f1 = 1; return; }
    uint32_t a, b, c, d;
    fib_pair(n >> 1, mod, &a, &b);
    uint64_t two_b = (uint64_t)b * 2 % mod;
    uint64_t a64 = a;
    uint64_t b64 = b;
    uint64_t c64 = a64 * ((two_b + mod - a64) % mod) % mod;
    uint64_t d64 = (a64 * a64 + b64 * b64) % mod;
    if ((n & 1) == 0) {
        *f  = (uint32_t)c64;
        *f1 = (uint32_t)d64;
    } else {
        *f  = (uint32_t)d64;
        *f1 = (uint32_t)((c64 + d64) % mod);
    }
}

static inline uint32_t fib_mod(uint32_t n, uint32_t mod)
{
    uint32_t f, f1;
    fib_pair(n, mod, &f, &f1);
    return f;
}

/* --------------------- Legendre sum and v_p --------------------- */
static inline uint64_t legendre_sum(uint64_t x, uint32_t p)
{
    uint64_t s = 0;
    while (x) { x /= p; s += x; }
    return s;
}

static inline uint32_t v_p_of(uint64_t n, uint32_t p)
{
    uint32_t c = 0;
    while (n % p == 0) { n /= p; ++c; }
    return c;
}

/* --------------------- Sieve and primes --------------------- */
#define MAX_LIMIT 200000

static uint32_t spf[MAX_LIMIT + 1];          /* smallest prime factor */
static uint32_t rank_arr[MAX_LIMIT + 1];     /* rank of appearance in Fibonacci */
static uint32_t primes_list[150000];
static size_t primes_cnt = 0;

static inline void factorize(uint32_t n, uint32_t *factors, uint32_t *exps, size_t *cnt)
{
    *cnt = 0;
    while (n > 1) {
        uint32_t p = spf[n];
        uint32_t e = 0;
        while (n % p == 0) { n /= p; ++e; }
        factors[(*cnt)]     = p;
        exps[(*cnt)]        = e;
        (*cnt)++;
    }
}

/* generate all divisors of a number given its prime factorization,
   store in divs array (size big enough). Return count. */
static size_t generate_divisors(const uint32_t *factors, const uint32_t *exps,
                                size_t cnt, uint32_t *divs, size_t maxcnt)
{
    size_t dcnt = 1;
    divs[0] = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint32_t p = factors[i];
        uint32_t e = exps[i];
        size_t cur = dcnt;
        uint32_t mult = 1;
        for (uint32_t k = 0; k < e; ++k) {
            mult *= p;
            for (size_t j = 0; j < cur; ++j) {
                if (dcnt >= maxcnt) return dcnt; /* safety */
                divs[dcnt++] = divs[j] * mult;
            }
        }
    }
    return dcnt;
}

/* rank of appearance of prime p in Fibonacci numbers (p>=2) */
static uint32_t rank_of_prime(uint32_t p)
{
    if (p == 2) return 3;
    if (p == 5) return 5;
    uint64_t leg = mod_pow(5, (p - 1) >> 1, p);
    uint32_t period = (leg == 1) ? (p - 1) : (p + 1);

    /* factor period */
    uint32_t factor[20], exp[20];
    size_t fcnt;
    factorize(period, factor, exp, &fcnt);

    /* generate divisors */
    uint32_t divs[2000];
    size_t dcnt = generate_divisors(factor, exp, fcnt, divs, 2000);
    sort_uint32(divs, dcnt);

    /* find smallest divisor d with F(d) ≡ 0 (mod p) */
    for (size_t i = 0; i < dcnt; ++i) {
        uint32_t d = divs[i];
        if (fib_mod(d, p) == 0) return d;
    }
    return period; /* fallback, should never happen */
}

/* --------------------- Global initialization --------------------- */
static void init_globals(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* sieve for spf and primes */
    for (uint32_t i = 2; i <= MAX_LIMIT; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if ((uint64_t)i * i <= MAX_LIMIT) {
                for (uint32_t j = i * i; j <= MAX_LIMIT; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
        primes_list[primes_cnt++] = i;
    }
    /* compute rank for all primes up to LIMIT */
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes_list[i];
        rank_arr[p] = rank_of_prime(p);
    }
    rank_arr[0] = rank_arr[1] = 0;
}

/* --------------------- Term test --------------------- */
static bool is_term(idx_t k)
{
    uint32_t temp = (uint32_t)k;
    while (temp > 1) {
        uint32_t p = spf[temp];
        uint32_t e = 0;
        while (temp % p == 0) { temp /= p; ++e; }
        uint32_t r = rank_arr[p];
        if (!r) return true;   /* should not occur */

        uint32_t M = (uint32_t)(k / r);
        uint64_t sum_fact = legendre_sum(M, p);

        uint32_t delta;
        if ((k - 1) % r == 0) {
            uint32_t m = (uint32_t)((k - 1) / r);
            delta = 1 + v_p_of(m, p);
        } else delta = 0;

        uint64_t exp_total = (uint64_t)M + sum_fact - delta;
        if (exp_total < e) return true;
    }
    return false;
}

/* --------------------- Cache for sequence values --------------------- */
static seq_t *terms = NULL;
static size_t terms_cnt = 0;
static idx_t next_k = 2;  /* next k candidate to test */

/* --------------------- Public function --------------------- */
seq_t a(idx_t n)
{
    init_globals();

    /* allocate cache if necessary */
    if (!terms) {
        size_t cap = (n > 1024) ? n : 1024;
        terms = (seq_t*)malloc(cap * sizeof(seq_t));
        terms_cnt = 0;
    }

    /* generate terms until we have n of them */
    while (terms_cnt < (size_t)n) {
        if (is_term(next_k)) {
            if (terms_cnt + 1 > terms_cnt) {             /* overflow guard */
                terms[terms_cnt++] = (seq_t)next_k;
            } else {
                /* shouldn't happen */
                terms[terms_cnt++] = (seq_t)next_k;
            }
        }
        ++next_k;
    }
    return terms[n - 1];
}
