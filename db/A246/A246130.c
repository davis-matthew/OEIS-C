/*
 * A246130  Binomial(2n,n)-2 mod n.
 * https://oeis.org/A246130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (binomial(2*n, n) - 2) mod n (offset: 1)
 * Range:     u8 n <= 264, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --- Helper: modular exponentiation ------------------------------------ */
static seq_t pow_mod(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1;
    seq_t b = base % mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1;
    }
    return result;
}

/* --- Helper: Legendre exponent of p in n! ------------------------------ */
static idx_t legendre(idx_t n, idx_t p)
{
    idx_t exp = 0;
    while (n) {
        n /= p;
        exp += n;
    }
    return exp;
}

/* --- Primes cache ------------------------------------------------------- */
static uint64_t *primes = NULL;     /* array of primes */
static size_t prime_cnt = 0;        /* number of primes stored */
static idx_t prime_limit = 0;       /* largest number sieved up to */

/* Ensure that `primes` contains all primes <= limit */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Simple Eratosthenes sieve up to 'limit' */
    size_t sz = (size_t)limit + 1;
    bool *is_prime = (bool*)calloc(sz, sizeof(bool));
    if (!is_prime)
        return;                       /* allocation failed – fall back to no primes */

    for (size_t i = 2; i < sz; ++i)
        is_prime[i] = true;

    for (size_t p = 2; p * p <= (size_t)limit; ++p)
        if (is_prime[p])
            for (size_t j = p * p; j <= (size_t)limit; j += p)
                is_prime[j] = false;

    /* Count primes */
    size_t cnt = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i])
            ++cnt;

    uint64_t *new_primes = (uint64_t*)malloc(cnt * sizeof(uint64_t));
    if (!new_primes) {
        free(is_prime);
        return;
    }

    size_t idx = 0;
    for (size_t i = 2; i < sz; ++i)
        if (is_prime[i])
            new_primes[idx++] = (uint64_t)i;

    free(primes);
    free(is_prime);

    primes = new_primes;
    prime_cnt = cnt;
    prime_limit = limit;
}

/* --- Main sequence function --------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* offset is 1 then */

    seq_t mod = (seq_t)n;
    seq_t binom_mod_n = 1;

    /* Prepare primes up to 2*n for Legendre factor extraction */
    ensure_primes(n * 2);

    for (size_t i = 0; i < prime_cnt && primes[i] <= 2 * n; ++i) {
        idx_t p = primes[i];
        idx_t exp = legendre(2 * n, p) - 2 * legendre(n, p);
        if (exp == 0)
            continue;
        seq_t factor = pow_mod((seq_t)p, exp, mod);
        binom_mod_n = (binom_mod_n * factor) % mod;
    }

    seq_t result = (binom_mod_n + mod - ((seq_t)2 % mod)) % mod;
    return result;
}
