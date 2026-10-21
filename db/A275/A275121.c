/*
 * A275121  a(n) is the smallest multiple of n that is a practical number.
 * https://oeis.org/A275121
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest multiple of n that is a practical number.
 *            Algorithm: for each n>1 test multiples n*k (k=1,2,…) in
 *            increasing order until a practical number is found. Practicality
 *            is checked by verifying the known condition for practical
 *            numbers: for the prime factorisation sorted increasingly p₁<...
 * Range:     u8 n <= 22, u16 n <= 408, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_pow.h"

static uint64_t *prime_list = NULL;
static size_t prime_cnt = 0;
static bool primes_inited = false;

/* ----------------------------------------------------------------- */
/* Simple Sieve: generate all primes up to LIMIT.                     */
/* ----------------------------------------------------------------- */
static void init_primes(void)
{
    if (primes_inited) return;
    const size_t LIMIT = 4000000;          /* sqrt(max potential candidate) */
    uint8_t *sieve = (uint8_t*)malloc(LIMIT + 1);
    for (size_t i = 0; i <= LIMIT; ++i) sieve[i] = 1;
    sieve[0] = sieve[1] = 0;
    for (size_t i = 2; i * i <= LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= LIMIT; j += i) sieve[j] = 0;

    /* count primes */
    for (size_t i = 2; i <= LIMIT; ++i) if (sieve[i]) ++prime_cnt;

    prime_list = (uint64_t*)malloc(prime_cnt * sizeof(uint64_t));
    size_t idx = 0;
    for (size_t i = 2; i <= LIMIT; ++i)
        if (sieve[i]) prime_list[idx++] = (uint64_t)i;

    free(sieve);
    primes_inited = true;
}

/* ----------------------------------------------------------------- */
/* Simple prime factorisation (trial division using the sieve).       */
/* ----------------------------------------------------------------- */
static void factor_ul(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt)
{
    *cnt = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i) {
        if (n % prime_list[i] == 0) {
            uint64_t e = 0;
            while (n % prime_list[i] == 0) {
                n /= prime_list[i];
                ++e;
            }
            pr[*cnt]   = prime_list[i];
            ex[*cnt]   = e;
            ++(*cnt);
        }
    }
    if (n > 1) {
        pr[*cnt] = n;
        ex[*cnt] = 1;
        ++(*cnt);
    }
}

/* ----------------------------------------------------------------- */
/* Sum of divisors of p^e.  (uses 128‑bit arithmetic).                */

/* ----------------------------------------------------------------- */
/* Practicality test for a 64‑bit unsigned integer.                   */
/* ----------------------------------------------------------------- */
static bool is_practical(uint64_t m)
{
    if (m == 1) return true;
    uint64_t pr[64], ex[64];
    size_t cnt;
    factor_ul(m, pr, ex, &cnt);

    /* The primes returned by factor_ul are already in increasing order. */
    unsigned __int128 sigma_pref = 1;      /* product of σ(p^e) of processed primes */

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        if (p > (uint64_t)(sigma_pref + 1)) return false;    /* failure condition */
        unsigned __int128 sigma = sigma_pow(p, ex[i]);
        sigma_pref *= sigma;    /* update prefix for next prime */
    }
    return true;
}

/* ----------------------------------------------------------------- */
/* Main routine requested by the problem statement.                  */
/* ----------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;            /* offset start */

    init_primes();                          /* prime list ready */

    uint64_t base = (uint64_t)n;            /* current candidate is base * k */
    for (uint64_t k = 1; ; ++k) {
        unsigned __int128 candidate128 = (unsigned __int128)base * k;
        /* if overflow would go beyond 64‑bit, stop (should not occur for tested range) */
        if (candidate128 > UINT64_MAX) break;
        uint64_t candidate = (uint64_t)candidate128;
        if (is_practical(candidate))
            return (seq_t)candidate;
    }
    /* should never reach here for inputs within the intended range */
    return (seq_t)0;
}
