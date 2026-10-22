/*
 * A385180  Primes p == 3 (mod 4) such that (p+1) * ord(5,p) / ord(2+-i,p) is
 *          divisible by 4. Here ord(a,m) is the multiplicative order of a
 *          modulo m.
 * https://oeis.org/A385180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 424, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   jacobi_symbol, mul_mod_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A385180:  Primes p ≡ 3 (mod 4) such that
 * ((p+1) * ord(5,p)) / ord(2+i,p) is divisible by 4.
 * seq_t is the built‑in numeric type for the sequence value.
 * idx_t is the built‑in unsigned integer type for the index. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/jacobi_symbol.h"   /* int jacobi_symbol(uint64_t,uint64_t) */
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t) */
#include "../utils/mul_mod_u64.h"

static uint64_t pow_mod_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp)
    {
        if (exp & 1ULL)
            res = mul_mod_u64(res, base, mod);
        base = mul_mod_u64(base, base, mod);
        exp >>= 1ULL;
    }
    return res;
}

/* trial division to list the *distinct* prime factors of n */
static void factor_distinct(uint64_t n, uint64_t out[], int *cnt)
{
    *cnt = 0;
    if (n < 2) return;
    if (n % 2ULL == 0ULL)
    {
        out[(*cnt)++] = 2ULL;
        while (n % 2ULL == 0ULL) n /= 2ULL;
    }
    for (uint64_t d = 3; d <= n / d; d += 2)
    {
        if (n % d == 0ULL)
        {
            out[(*cnt)++] = d;
            while (n % d == 0ULL) n /= d;
        }
    }
    if (n > 1ULL) out[(*cnt)++] = n;
}

/* multiplicative order of a modulo prime m (m is prime) */
static uint64_t order_mod(uint64_t a, uint64_t m)
{
    uint64_t phi = m - 1ULL;
    uint64_t fac[64];
    int fc;
    factor_distinct(phi, fac, &fc);

    uint64_t ord = phi;
    for (int i = 0; i < fc; ++i)
    {
        uint64_t p = fac[i];
        while ((ord % p == 0ULL) && (pow_mod_u64(a, ord / p, m) == 1ULL))
            ord /= p;
    }
    return ord;
}

/* Gaussian integer element modulo p (with i² = –1) */
typedef struct { uint64_t a, b; } gf_elem;

static inline gf_elem gf_mul(const gf_elem x, const gf_elem y, uint64_t p)
{
    uint64_t ax = mul_mod_u64(x.a, y.a, p);
    uint64_t bx = mul_mod_u64(x.b, y.b, p);

    uint64_t a_res = ax;
    uint64_t sub = bx;
    if (a_res >= sub)
        a_res -= sub;
    else
        a_res = a_res + p - sub;

    uint64_t b_res = (mul_mod_u64(x.a, y.b, p) + mul_mod_u64(x.b, y.a, p)) % p;
    return (gf_elem){ a_res, b_res };
}

static gf_elem gf_pow(gf_elem base, uint64_t exp, uint64_t p)
{
    gf_elem res = { 1 % p, 0 };
    while (exp)
    {
        if (exp & 1ULL)
            res = gf_mul(res, base, p);
        base = gf_mul(base, base, p);
        exp >>= 1ULL;
    }
    return res;
}

/* order of (2+i) in (ℤ/pℤ)[i] */
static uint64_t order_gaussian(uint64_t p)
{
    /* collect distinct prime factors of (p-1) and (p+1) */
    uint64_t primes[128];
    int pcnt1, pcnt2, pcnt;
    factor_distinct(p - 1ULL, primes, &pcnt1);
    factor_distinct(p + 1ULL, primes + pcnt1, &pcnt2);
    pcnt = pcnt1 + pcnt2;

    unsigned __int128 m128 = (unsigned __int128)p * p - 1ULL;
    uint64_t m = (uint64_t)m128;
    uint64_t ord = m;
    gf_elem base = { 2ULL % p, 1ULL % p };

    for (int i = 0; i < pcnt; ++i)
    {
        uint64_t q = primes[i];
        while ((ord % q == 0ULL) &&
               (gf_pow(base, ord / q, p).a == 1 % p) &&
               (gf_pow(base, ord / q, p).b == 0ULL))
            ord /= q;
    }
    return ord;
}

/* memoised sequence */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;
static uint64_t last_checked = 1ULL;   /* next prime to consider */

seq_t a(idx_t n)
{
    while (cache_len < n)
    {
        last_checked = next_prime(last_checked);
        if (last_checked % 4ULL != 3ULL)
            continue;

        /* 5 must be a quadratic residue modulo p */
        if (jacobi_symbol(5ULL, last_checked) != 1)
            continue;

        uint64_t p = last_checked;
        uint64_t ord5  = order_mod(5ULL, p);
        uint64_t ord2i = order_gaussian(p);

        unsigned __int128 numerator = (unsigned __int128)(p + 1ULL) * ord5;
        uint64_t ratio = (uint64_t)(numerator / ord2i);

        if (ratio % 4ULL == 0ULL)
        {
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 64;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)p;
        }
    }
    return cache[n - 1];
}
