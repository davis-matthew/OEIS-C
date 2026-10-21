/*
 * A283597  Numbers n such that both 6n+1 and 6n+7 are semiprimes.
 * https://oeis.org/A283597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that both 6n+1 and 6n+7 are semiprimes (OEIS A283597). */
/* The routine searches for the required n by checking each integer in turn. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* ---------- Helper: prime sieve up to a fixed limit ---------- */

static uint64_t *prime_list = NULL;   /* primes up to LIMIT */
static size_t      prime_cnt  = 0;     /* number of primes found */
static bool        primes_ok   = false;/* flag indicating primes prepared */

static void init_primes(void)
{
    if (primes_ok) return;

    /* sufficient limit for expected n – 200 000 gives ~ 18 000 primes */
    const size_t LIMIT = 200000;
    bool *is_prime = (bool *)malloc((LIMIT + 1) * sizeof(bool));
    if (!is_prime) abort();
    for (size_t i = 0; i <= LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= LIMIT; j += i) is_prime[j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) ++cnt;

    prime_list = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!prime_list) abort();
    size_t pos = 0;
    for (size_t i = 2; i <= LIMIT; ++i) if (is_prime[i]) prime_list[pos++] = (uint64_t)i;

    free(is_prime);
    prime_cnt = cnt;
    primes_ok = true;
}

/* ---------- Smallest prime factor using the pre‑computed primes ---------- */

static uint64_t smallest_prime_factor(uint64_t x)
{
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p * p > x) break;
        if (x % p == 0) return p;
    }
    return x; /* x is prime */
}

/* ---------- Semiprime test ---------- */

static bool is_semiprime(uint64_t x)
{
    if (x < 4) return false;
    uint64_t p = smallest_prime_factor(x);
    if (p == x) return false;      /* x itself is prime */
    uint64_t q = x / p;
    return smallest_prime_factor(q) == q;
}

/* ---------- Sequence generator ---------- */

seq_t a(idx_t n)
{
    /* static cache of previously discovered terms */
    static seq_t *vals      = NULL;
    static size_t val_len   = 0;
    static size_t val_cap   = 0;
    static idx_t  next_n    = 1;   /* next integer to test for the property */

    init_primes();

    while (val_len < n) {
        uint64_t n_val = (uint64_t)next_n;
        uint64_t x1 = 6 * n_val + 1;
        uint64_t x2 = 6 * n_val + 7;

        if (is_semiprime(x1) && is_semiprime(x2)) {
            if (val_len == val_cap) {
                val_cap = val_cap ? val_cap * 2 : 64;
                vals = (seq_t *)realloc(vals, val_cap * sizeof(seq_t));
                if (!vals) abort();
            }
            vals[val_len++] = (seq_t)n_val;
        }
        ++next_n;
    }
    return vals[n - 1];
}
