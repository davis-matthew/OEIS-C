/*
 * A385166  Let p = A002145(n) be the n-th prime == 3 (mod 4); a(n) = (p+1) *
 *          ord(5,p) / ord(2+-i,p) = (p+1) * ord(5,p) / A385165(n). Here
 *          ord(a,m) is the multiplicative order of a modulo m.
 * https://oeis.org/A385166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (p+1)*ord(5,p)/ord(2±i,p) where p is the n-th prime ≡ 3
 *            (mod 4)
 * Range:     u8 n <= 553, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gaussian_mul, gaussian_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --- Dependencies and standard headers ----------------------------------------------------- */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gaussian_mul.h"
#include "../utils/gaussian_pow.h"

/* --- Small prime generation (up to a fixed limit) -------------------------------------------- */
#define SMALL_PRIME_LIMIT 400000   /* sufficient for all n up to about 10000 */

static uint32_t *small_primes = NULL;   /* dynamic array of primes */
static size_t   small_prime_cnt = 0;

/* sieve of Eratosthenes to build small_primes */
static void init_small_primes(void)
{
    if (small_primes) return;   /* already initialized */

    uint32_t *is_composite = (uint32_t *)calloc(SMALL_PRIME_LIMIT + 1, sizeof(uint32_t));
    size_t cnt = 0;
    for (uint32_t i = 2; i <= SMALL_PRIME_LIMIT; ++i)
        if (!is_composite[i]) {
            ++cnt;
            if ((uint64_t)i * i <= SMALL_PRIME_LIMIT)
                for (uint32_t j = i * i; j <= SMALL_PRIME_LIMIT; j += i)
                    is_composite[j] = 1;
        }

    small_primes = (uint32_t *)malloc(cnt * sizeof(uint32_t));
    size_t idx = 0;
    for (uint32_t i = 2; i <= SMALL_PRIME_LIMIT; ++i)
        if (!is_composite[i]) small_primes[idx++] = i;

    small_prime_cnt = cnt;
    free(is_composite);
}

/* --- Simple primality test using the precomputed small primes ------------------------------ */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) return false;
    }
    return true;
}

/* --- Next prime > start (using is_prime_uint64) -------------------------------------------- */
static uint64_t next_prime_uint64(uint64_t start)
{
    if (start <= 2) return 2;
    /* make start odd */
    if ((start & 1ULL) == 0) ++start;
    while (!is_prime_uint64(start))
        start += 2;
    return start;
}

/* --- Cache of primes ≡ 3 (mod 4) --------------------------------------------------------- */
static uint64_t *pr3 = NULL;          /* array of primes ≡3 mod 4 */
static size_t    pr3_len = 0;          /* number of stored primes */
static size_t    pr3_cap = 0;          /* current capacity */
static uint64_t  last_checked_prime = 2;   /* last prime examined */

/* ensures that pr3 holds at least n primes (n >=1) */
static void ensure_pr3(size_t n)
{
    if (pr3_len >= n) return;
    for (;;) {
        uint64_t cand = next_prime_uint64(last_checked_prime + 1);
        last_checked_prime = cand;
        if (cand % 4 == 3) {
            if (pr3_len == pr3_cap) {
                size_t newcap = pr3_cap ? pr3_cap * 2 : 16;
                uint64_t *tmp = (uint64_t*)realloc(pr3, newcap * sizeof(uint64_t));
                if (!tmp) exit(EXIT_FAILURE);
                pr3 = tmp;
                pr3_cap = newcap;
            }
            pr3[pr3_len++] = cand;
            if (pr3_len >= n) break;
        }
    }
}

/* --- Modular exponentiation for 64‑bit integers ------------------------------------------- */
static uint64_t mod_pow_u64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1ULL) {
            __uint128_t t = (__uint128_t)result * b % mod;
            result = (uint64_t)t;
        }
        exp >>= 1;
        if (exp) {
            __uint128_t t = (__uint128_t)b * b % mod;
            b = (uint64_t)t;
        }
    }
    return result;
}

/* --- Factorization into distinct prime factors (small primes assumed enough) ----------------- */
static void factor_distinct(uint64_t n, uint64_t *factors, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < small_prime_cnt; ++i) {
        uint32_t p = small_primes[i];
        if ((uint64_t)p * p > n) break;
        if (n % p == 0) {
            factors[(*cnt)++] = p;
            while (n % p == 0) n /= p;
        }
    }
    if (n > 1) factors[(*cnt)++] = n;
}

/* --- Return principal nth prime ≡ 3 (mod 4) ------------------------------------------------ */
static uint64_t get_p3(size_t n)
{
    ensure_pr3(n);
    return pr3[n - 1];
}

/* --- Compute ord(5,p) --------------------------------------------------------------------- */
static uint64_t ord5(uint64_t p)
{
    uint64_t n = p - 1;
    uint64_t prime_factors[64];
    size_t cnt;

    factor_distinct(n, prime_factors, &cnt);

    uint64_t ord = n;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = prime_factors[i];
        while (ord % q == 0) {
            uint64_t test = ord / q;
            if (mod_pow_u64(5, test, p) == 1)
                ord = test;
            else
                break;
        }
    }
    return ord;
}

/* --- Compute ord(2+i,p) --------------------------------------------------------------- */
static uint64_t ord2i(uint64_t p)
{
    uint64_t n1 = p - 1;
    uint64_t n2 = p + 1;

    /* collect distinct prime factors of n1 and n2 */
    uint64_t primes_combined[128];
    size_t len = 0;
    uint64_t temp_f[64];
    size_t cnt;

    factor_distinct(n1, temp_f, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = temp_f[i];
        bool found = false;
        for (size_t j = 0; j < len; ++j)
            if (primes_combined[j] == q) { found = true; break; }
        if (!found) primes_combined[len++] = q;
    }

    factor_distinct(n2, temp_f, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = temp_f[i];
        bool found = false;
        for (size_t j = 0; j < len; ++j)
            if (primes_combined[j] == q) { found = true; break; }
        if (!found) primes_combined[len++] = q;
    }

    /* ord starts as (p-1)*(p+1) = p^2 - 1 */
    uint64_t ord = (uint64_t)n1 * n2;   /* fits in 64 bits for all n needed */

    /* try to reduce ord by each prime factor */
    for (size_t i = 0; i < len; ++i) {
        uint64_t q = primes_combined[i];
        while (ord % q == 0) {
            uint64_t test = ord / q;
            uint64_t a, b;
            gaussian_pow(2 % p, 1 % p, test, p, &a, &b);
            if (a == 1 % p && b == 0)  /* exact match */
                ord = test;
            else
                break;
        }
    }
    return ord;
}

/* --- Main sequence function -------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined below offset, return 0 */

    init_small_primes();                  /* make sure prime tables exist */

    uint64_t p = get_p3((size_t)n);       /* n-th prime ≡ 3 mod 4 */

    uint64_t o5  = ord5(p);
    uint64_t o2i = ord2i(p);

    /* numerator: (p+1)*o5, safe in 128 bits */
    __uint128_t num128 = (__uint128_t)(p + 1) * o5;
    uint64_t num = (uint64_t)num128;

    uint64_t result = num / o2i;          /* division is exact by definition */

    return (seq_t)result;
}
