/*
 * A272475  Numbers n such that 2^n-1 and 3^n-1 are not coprime.
 * https://oeis.org/A272475
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th positive integer n such that gcd(2^n-1, 3^n-1)
 *            > 1
 * Range:     u8 n <= 144, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* seq_t and idx_t are provided by the build system. */
#include "../utils/gcd.h"
#include "../utils/mul_small.h"

/* --------------------------------------------------------------------- */
/*  Various helpers for prime treatment and modular arithmetic (64‑bit). */
/* --------------------------------------------------------------------- */
static const idx_t MAX_PRIME = 200000;               /* bounds for prime search */

static bool primes_ready = false;
static idx_t *prime_list = NULL;
static size_t prime_count = 0;

static void build_prime_list(void)
{
    if (primes_ready) return;
    primes_ready = true;

    bool *is_composite = (bool*)calloc(MAX_PRIME + 1, sizeof(bool));
    for (idx_t i = 2; i * i <= MAX_PRIME; ++i) {
        if (!is_composite[i]) {
            for (idx_t j = i * i; j <= MAX_PRIME; j += i)
                is_composite[j] = true;
        }
    }
    prime_count = 0;
    for (idx_t i = 2; i <= MAX_PRIME; ++i)
        if (!is_composite[i]) {
            ++prime_count;
        }
    prime_list = (idx_t*)malloc(prime_count * sizeof(idx_t));
    size_t pos = 0;
    for (idx_t i = 2; i <= MAX_PRIME; ++i)
        if (!is_composite[i])
            prime_list[pos++] = i;
    free(is_composite);
}

/* 64‑bit modular exponentiation (base < mod).  */
static idx_t powmod_u64(idx_t base, idx_t exp, idx_t mod)
{
    idx_t res = 1 % mod;
    idx_t cur = base % mod;
    while (exp) {
        if (exp & 1)
            res = (idx_t)((__uint128_t)res * cur % mod);
        cur = (idx_t)((__uint128_t)cur * cur % mod);
        exp >>= 1;
    }
    return res;
}

/* Compute the order of a modulo prime p (p>3). */
static idx_t order_mod(idx_t a, idx_t p)
{
    idx_t ord = p - 1;
    idx_t t   = p - 1;
    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= t; ++i) {
        idx_t q = prime_list[i];
        if (t % q == 0) {
            while (t % q == 0) t /= q;
            while (ord % q == 0 && powmod_u64(a, ord / q, p) == 1)
                ord /= q;
        }
    }
    if (t > 1) {
        idx_t q = t;
        while (ord % q == 0 && powmod_u64(a, ord / q, p) == 1)
            ord /= q;
    }
    return ord;
}

/* GCD of two 64‑bit integers. */
static idx_t gcd64(idx_t a, idx_t b)
{
    while (b) { idx_t t = a % b; a = b; b = t; }
    return a;
}

/* --------------------------------------------------------------------- */
/*  Precomputation of L = lcm(ord_2, ord_3) for primes >= 5.            */
/* --------------------------------------------------------------------- */
static bool Lvals_ready = false;
static idx_t *L_vals = NULL;
static size_t L_len   = 0;
static size_t L_cap   = 0;

static void compute_Lvals(void)
{
    if (Lvals_ready) return;
    Lvals_ready = true;

    build_prime_list();

    /* Estimate number of primes >=5 up to MAX_PRIME. */
    L_cap = prime_count + 10;
    L_vals = (idx_t*)malloc(L_cap * sizeof(idx_t));
    L_len = 0;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p < 5) continue;               /* 2 and 3 cannot divide both numbers */

        idx_t ord2 = order_mod(2, p);
        idx_t ord3 = order_mod(3, p);
        idx_t g = gcd64(ord2, ord3);
        idx_t lcm = (ord2 / g) * ord3;

        if (L_len == L_cap) {
            L_cap *= 2;
            L_vals = (idx_t*)realloc(L_vals, L_cap * sizeof(idx_t));
        }
        L_vals[L_len++] = lcm;
    }
}

/* --------------------------------------------------------------------- */
/*  Cache of the sequence values a(1), a(2), … a(max_cached_len).      */
/* --------------------------------------------------------------------- */
static seq_t *seq_cache      = NULL;
static size_t seq_len        = 0;   /* number of cached terms */
static idx_t  cached_max_n   = 0;   /* highest n for which a(n) is cached */

static void update_cache(idx_t need_n)
{
    if (need_n <= cached_max_n) return;   /* already cached */

    compute_Lvals();        /* ensure L_vals is ready */

    /* Allocate new array for terms (wide enough for need_n). */
    seq_t *new_cache = (seq_t*)malloc((need_n + 1) * sizeof(seq_t));
    size_t new_len = 0;

    /* Choose an upper bound M such that we are sure to find need_n terms. */
    idx_t M = need_n * 4;            /* heuristic */
    if (M < 4) M = 4;

    while (true) {
        /* Boolean array marking members of the sequence. */
        uint8_t *good = (uint8_t*)calloc((size_t)(M + 1), sizeof(uint8_t));
        if (!good) { free(new_cache); return; }

        /* Mark multiples of each L. */
        for (size_t i = 0; i < L_len; ++i) {
            idx_t L = L_vals[i];
            if (L == 0) continue;
            for (idx_t m = L; m <= M; m += L)
                good[m] = 1;
        }

        /* Collect terms until we have need_n ones. */
        for (idx_t m = 1; m <= M && new_len < need_n; ++m) {
            if (good[m]) new_cache[new_len++] = (seq_t)m;
        }

        free(good);

        if (new_len >= need_n)
            break;

        /* Not enough terms – increase the bound and try again. */
        M *= 2;
    }

    /* Replace old cache. */
    free(seq_cache);
    seq_cache = new_cache;
    seq_len   = new_len;
    cached_max_n = need_n;
}

/* --------------------------------------------------------------------- */
/*  Public function defined by the assignment.                            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined but harmless */
    update_cache(n);
    return seq_cache[n - 1];
}
