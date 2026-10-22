/*
 * A334487  a(n) = p(n, 2)*p(n, 5)/p(n, 10) where p(n, b) is the period of
 *          repeating digits of 1/n in base b.
 * https://oeis.org/A334487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = p(n,2)*p(n,5)/p(n,10)
 * Range:     u8 n <= 36, u16 n <= 2150, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   remove_factors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* p(n,b) = period of 1/n in base b, i.e. multiplicative order of b modulo n',
   where n' is n with all factors of b removed. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/remove_factors.h"

/* maximum prime for pre‑computed sieve (covers sqrt of n up to ~1e12) */
#define MAX_PRIME 1000000

/* prime table */
static bool prime_sieve[MAX_PRIME + 1];
static uint32_t primes[80000];          /* number of primes <= 1e6 is 78498 */
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* initialise prime table once */
static void init_primes(void)
{
    if (primes_inited) return;
    for (size_t i = 0; i <= MAX_PRIME; ++i) prime_sieve[i] = true;
    prime_sieve[0] = prime_sieve[1] = false;
    for (size_t i = 2; i * i <= MAX_PRIME; ++i)
        if (prime_sieve[i])
            for (size_t j = i * i; j <= MAX_PRIME; j += i)
                prime_sieve[j] = false;

    prime_cnt = 0;
    for (size_t i = 2; i <= MAX_PRIME; ++i)
        if (prime_sieve[i]) primes[prime_cnt++] = (uint32_t)i;
    primes_inited = true;
}

/* modular multiplication (base on 64 bits) */
static uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (__uint128_t)a * b % mod;
}

/* modular exponentiation */
static uint64_t powmod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1) result = mul_mod_uint64(result, base, mod);
        base = mul_mod_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* multiplicative order of 'base' modulo 'mod'  (mod>0, gcd(base,mod)=1) */
static uint64_t multiplicative_order(uint64_t base, uint64_t mod)
{
    if (mod == 1) return 1;
    init_primes();

    /* compute Euler's phi(mod) */
    uint64_t phi = mod;
    uint64_t m = mod;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > m) break;
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            phi = phi / p * (p - 1);
        }
    }
    if (m > 1) phi = phi / m * (m - 1);

    /* factor phi to reduce the order */
    uint64_t order = phi;
    uint64_t t = phi;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        if ((uint64_t)p * p > t) break;
        if (t % p == 0) {
            while (t % p == 0) t /= p;
            while (order % p == 0 && powmod_uint64(base, order / p, mod) == 1U)
                order /= p;
        }
    }
    if (t > 1) {
        while (order % t == 0 && powmod_uint64(base, order / t, mod) == 1U)
            order /= t;
    }
    return order;
}

/* sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined below offset, but safe */

    uint64_t nn = (uint64_t)n;

    /* period in base 2 */
    uint64_t m2 = remove_factors(nn, 2ull);
    uint64_t p2 = (m2 == 1) ? 1 : multiplicative_order(2ull, m2);

    /* period in base 5 */
    uint64_t m5 = remove_factors(nn, 5ull);
    uint64_t p5 = (m5 == 1) ? 1 : multiplicative_order(5ull, m5);

    /* period in base 10 */
    uint64_t m10 = remove_factors(remove_factors(nn, 2ull), 5ull);
    uint64_t p10 = (m10 == 1) ? 1 : multiplicative_order(10ull, m10);

    /* compute result (ensure no overflow of intermediate 64‑bit) */
    uint64_t prod = p2 * p5;
    uint64_t res = prod / p10;
    return (seq_t)res;
}
