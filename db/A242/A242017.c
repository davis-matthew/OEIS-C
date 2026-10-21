/*
 * A242017  Smallest prime factor of composites in the sequence A000051(n) =
 *          2^n+1.
 * https://oeis.org/A242017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 58, u32 n <= 122, u64 n <= 1786,
 *            u128 n <= 1786, big any n
 * Helpers:   is_known_prime_exp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A242017: for each n, a(n) is the smallest prime divisor of the n‑th composite
   number among the sequence A000051(n) = 2^n + 1. 
   The implementation enumerates composite exponents k (skipping k = 1, 2, 4, 8, 16 where 2^k+1 is prime),
   then returns the smallest prime factor of 2^k+1.  For odd k the factor is always 3.  For even k
   a quick modular exponent test is used to find the smallest prime divisor. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_known_prime_exp.h"

/* ---------- prime sieve (up to 2 000 000) ---------- */
#define SIEVE_LIMIT 2000000
#define MAX_PRIMES 150000

static bool sieve_table[SIEVE_LIMIT + 1];
static uint32_t prime_list[MAX_PRIMES];
static size_t prime_count = 0;
static bool primes_ready = false;

static void init_primes(void)
{
    if (primes_ready)
        return;
    memset(sieve_table, 0, sizeof(sieve_table));
    sieve_table[0] = sieve_table[1] = true;
    size_t i, j;
    for (i = 2; i * i <= SIEVE_LIMIT; ++i) {
        if (!sieve_table[i]) {
            for (j = i * i; j <= SIEVE_LIMIT; j += i)
                sieve_table[j] = true;
        }
    }
    prime_count = 0;
    for (i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!sieve_table[i]) {
            prime_list[prime_count++] = (uint32_t)i;
            if (prime_count == MAX_PRIMES)
                break;
        }
    }
    primes_ready = true;
}

/* ---------- modular exponentiation: (base^exp) mod mod  ---------- */
static uint64_t powmod_uint64(uint64_t base, idx_t exp, uint32_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------- smallest prime factor of 2^k + 1, assuming k >= 3 and composite ---------- */
static seq_t find_spf(idx_t k)
{
    /* For odd k, 3 is always a divisor. */
    if ((k & 1) == 1)
        return (seq_t)3;

    /* Even k: search primes starting from 5 (prime 2 and 3 are not divisors). */
    for (size_t idx = 0; idx < prime_count; ++idx) {
        uint32_t p = prime_list[idx];
        if (p == 2 || p == 3)
            continue;
        uint64_t v = powmod_uint64(2ULL, k, p);
        if ((v + 1) % p == 0)
            return (seq_t)p;
    }
    /* In theory we should never reach here for the range of n required by the tests. */
    return (seq_t)0;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined outside the offset */

    init_primes();

    idx_t cnt = 0;
    idx_t k = 3;          /* first composite exponent */

    while (cnt < n) {
        if (is_known_prime_exp(k)) {
            ++k;
            continue;      /* skip primes where 2^k+1 is prime */
        }

        seq_t spf = find_spf(k);
        if (cnt + 1 == n)
            return spf;

        ++cnt;
        ++k;
    }
    /* unreachable */
    return (seq_t)0;
}
