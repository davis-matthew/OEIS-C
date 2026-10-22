/*
 * A371472  Least positive number k such that binomial(k^2,k) is divisible by
 *          n^2.
 * https://oeis.org/A371472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least positive k such that C(k^2,k) is divisible by n^2
 * Range:     u8 n <= 72, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* == A371472 == */
/* The function below factors n, then searches for the smallest k satisfying
   the exponent conditions for all prime factors of n. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#define SIEVE_LIMIT 100000          /* sufficient for n up to about 1e10 */

/* ---------- Prime sieve (once) ---------- */
static int primes[SIEVE_LIMIT + 1];
static size_t prime_cnt = 0;

static void init_sieve(void)
{
    static bool initialized = false;
    if (initialized) return;

    bool is_composite[SIEVE_LIMIT + 1];
    for (int i = 0; i <= SIEVE_LIMIT; ++i) is_composite[i] = false;

    for (int i = 2; (long long)i * i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            for (int j = i * i; j <= SIEVE_LIMIT; j += i) is_composite[j] = true;
        }
    }

    for (int i = 2; i <= SIEVE_LIMIT; ++i) {
        if (!is_composite[i]) {
            primes[prime_cnt++] = i;
        }
    }
    initialized = true;
}

/* ---------- Factorisation of n (using the sieve) ---------- */
static void factor_uint64(uint64_t n,
                          uint64_t *pf,   /* array of distinct primes */
                          int *exps,      /* corresponding exponents */
                          int *cnt)       /* number of distinct primes */
{
    *cnt = 0;
    uint64_t m = n;
    for (size_t i = 0; i < prime_cnt && (uint64_t)primes[i] * primes[i] <= m; ++i) {
        if (m % primes[i] == 0) {
            int e = 0;
            while (m % primes[i] == 0) {
                m /= primes[i];
                ++e;
            }
            pf[(*cnt)] = (uint64_t)primes[i];
            exps[(*cnt)] = e;
            ++(*cnt);
        }
    }
    if (m > 1) {                /* remaining prime factor */
        pf[(*cnt)] = m;
        exps[(*cnt)] = 1;
        ++(*cnt);
    }
}

/* ---------- Legendre exponent of a prime in n! ---------- */
static inline uint64_t v_p_factorial(uint64_t m, uint64_t p)
{
    uint64_t sum = 0;
    while (m) {
        m /= p;
        sum += m;
    }
    return sum;
}

/* ---------- Exponent of p in C(k^2,k) ---------- */
static inline uint64_t v_p_binom(uint64_t k, uint64_t p)
{
    uint64_t k2 = k * k;
    return v_p_factorial(k2, p) - v_p_factorial(k, p) - v_p_factorial(k2 - k, p);
}

/* ==================== Main sequence function ==================== */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    init_sieve();

    uint64_t N = (uint64_t)n;
    uint64_t prime_factors[20];
    int    exponents[20];
    int    num;
    factor_uint64(N, prime_factors, exponents, &num);

    /* Search for the smallest k satisfying all prime-power conditions */
    for (uint64_t k = 1; ; ++k) {
        bool ok = true;
        for (int i = 0; i < num; ++i) {
            uint64_t required = (uint64_t)exponents[i] * 2;
            if (v_p_binom(k, prime_factors[i]) < required) {
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)k;
    }
}
